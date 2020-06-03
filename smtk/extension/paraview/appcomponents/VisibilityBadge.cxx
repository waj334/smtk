//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#include "smtk/extension/paraview/appcomponents/VisibilityBadge.h"

#include "smtk/extension/paraview/appcomponents/pqSMTKBehavior.h"
#include "smtk/extension/paraview/appcomponents/pqSMTKRenderResourceBehavior.h"
#include "smtk/extension/paraview/appcomponents/pqSMTKResourceBrowser.h"
#include "smtk/extension/paraview/appcomponents/pqSMTKResourceRepresentation.h"
#include "smtk/extension/paraview/server/vtkSMTKResourceRepresentation.h" // FIXME: Remove the need for me

#include "smtk/extension/paraview/appcomponents/pqEyeballClosed_svg.h"
#include "smtk/extension/paraview/appcomponents/pqEyeball_svg.h"

#include "smtk/common/Color.h"
#include "smtk/mesh/core/Component.h"
#include "smtk/mesh/core/Resource.h"
#include "smtk/model/AuxiliaryGeometry.h"
#include "smtk/model/Entity.h"
#include "smtk/model/EntityIterator.h"
#include "smtk/model/EntityRef.h"
#include "smtk/model/Resource.h"
#include "smtk/view/BadgeSet.h"
#include "smtk/view/DescriptivePhrase.h"
#include "smtk/view/Manager.h"

#include "pqActiveObjects.h"
#include "vtkCompositeRepresentation.h"
#include "vtkSMProxy.h"

namespace smtk
{
namespace extension
{
namespace paraview
{
namespace appcomponents
{

template <typename T, typename U>
int UpdateVisibilityForFootprint(pqSMTKResourceRepresentation* smap, const T& comp, int visible,
  U& visibleThings, const smtk::view::DescriptivePhrasePtr& /*unused*/)
{
  bool didUpdate = false;
  int rval(0);

  if (auto ment = std::dynamic_pointer_cast<smtk::model::Entity>(comp))
  {
    if (ment->isModel() || ment->isGroup())
    {
      int any = 0;
      smtk::model::EntityIterator childIt;
      smtk::model::EntityRef entRef = ment->template referenceAs<smtk::model::EntityRef>();
      childIt.traverse(entRef, smtk::model::IteratorStyle::ITERATE_CHILDREN);
      for (childIt.begin(); !childIt.isAtEnd(); ++childIt)
      {
        auto child = childIt.current().entityRecord();
        int ok = smap->setVisibility(child, visible);
        any |= ok;
        visibleThings[child->id()] = visible;
      }
      rval = any;
      if (any)
      {
        didUpdate = true;
      }
    }
    else
    {
      // Composite auxliliary geometry condition
      int any = 0;
      smtk::model::AuxiliaryGeometry auxgeom =
        ment->template referenceAs<smtk::model::AuxiliaryGeometry>();
      auto auxgeomChildren = auxgeom.auxiliaryGeometries();
      if (auxgeom && !auxgeomChildren.empty())
      {
        for (const auto& child : auxgeomChildren)
        {
          int ok = smap->setVisibility(child.component(), visible != 0);
          any |= ok;
          visibleThings[child.entity()] = visible;
        }
      }
      rval |= any;

      rval |= smap->setVisibility(comp, visible != 0) ? 1 : 0;
      if (rval)
      {
        visibleThings[comp->id()] =
          visible; // Should we set here or wait until we hear back from smap?
        didUpdate = true;
      }
    }
  }
  else if (auto meshComponent = std::dynamic_pointer_cast<smtk::mesh::Component>(comp))
  {
    rval |= smap->setVisibility(comp, visible != 0) ? 1 : 0;
    if (rval)
    {
      visibleThings[comp->id()] = visible;
      didUpdate = true;
    }
  }

  if (didUpdate)
  {
    smap->renderViewEventually();
  }
  return rval;
}

/// Types of queries for visibility.
enum Query
{
  DISPLAYABLE,
  EDITABLE,
  GET_VALUE,
  SET_VALUE
};

int vizQuery(Query query, int val, const smtk::view::ConstPhraseContentPtr data,
  std::map<smtk::common::UUID, int>& visibleThings)
{
  auto comp = data->relatedComponent();
  auto rsrc = data->relatedResource();

  smtk::model::EntityPtr ent = std::dynamic_pointer_cast<smtk::model::Entity>(comp);
  smtk::model::ResourcePtr modelRsrc =
    ent ? ent->modelResource() : std::dynamic_pointer_cast<smtk::model::Resource>(rsrc);

  smtk::mesh::ComponentPtr msh = std::dynamic_pointer_cast<smtk::mesh::Component>(comp);
  smtk::mesh::ResourcePtr meshRsrc = msh
    ? std::dynamic_pointer_cast<smtk::mesh::Resource>(msh->resource())
    : std::dynamic_pointer_cast<smtk::mesh::Resource>(rsrc);

  auto smtkBehavior = pqSMTKBehavior::instance();

  // If we are trying to get the value of a resource that has no pipeline
  // source, we create one.
  auto pvrc = smtkBehavior->getPVResource(rsrc);
  if (pvrc == nullptr && rsrc)
  {
    pvrc = pqSMTKRenderResourceBehavior::instance()->createPipelineSource(rsrc);
  }

  // TODO: We could check more than just that the view is non-null.
  //       For instance, does the resource have a representation in the active view?
  //       However, that gets expensive.
  bool validView = pqActiveObjects::instance().activeView() != nullptr;

  switch (query)
  {
    case DISPLAYABLE:
      return validView && (ent || (!ent && modelRsrc) || (msh || (!ent && meshRsrc))) ? 1 : 0;
    case EDITABLE:
      return validView && (ent || (!ent && modelRsrc) || (msh || (!ent && meshRsrc))) ? 1 : 0;
    case GET_VALUE:
      if (ent || msh)
      {
        auto valIt = visibleThings.find(comp->id());
        if (valIt != visibleThings.end())
        {
          return valIt->second;
        }
        return 1; // visibility is assumed if there is no entry.
      }
      else if (modelRsrc || meshRsrc)
      {
        auto view = pqActiveObjects::instance().activeView();
        auto mapr = pvrc ? pvrc->getRepresentation(view) : nullptr;
        return mapr ? mapr->isVisible() : 0;
      }
      return 0; // visibility is false if the component is not a model entity or NULL.
    case SET_VALUE:
      if (ent || msh)
      { // Find the mapper in the active view for the related resource, then set the visibility.
        auto view = pqActiveObjects::instance().activeView();
        auto mapr = pvrc ? pvrc->getRepresentation(view) : nullptr;
        auto smap = dynamic_cast<pqSMTKResourceRepresentation*>(mapr);
        if (smap)
        {
          int rval = UpdateVisibilityForFootprint(smap, comp, val, visibleThings, data->location());
          return rval;
        }
      }
      else if (modelRsrc || meshRsrc)
      { // A resource, not a component, is being modified. Change the pipeline object's visibility.
        auto view = pqActiveObjects::instance().activeView();
        auto mapr = pvrc ? pvrc->getRepresentation(view) : nullptr;
        if (mapr)
        {
          mapr->setVisible(!mapr->isVisible());
          pqActiveObjects::instance().setActiveSource(pvrc);
          mapr->renderViewEventually();
          return 1;
        }
      }
      return 0;
  }
  return 0;
}

VisibilityBadge::VisibilityBadge()
  : m_icon(pqEyeball_svg)
  , m_iconClosed(pqEyeballClosed_svg)
  , m_parent(nullptr)
{
}
VisibilityBadge::VisibilityBadge(
  smtk::view::BadgeSet& parent, const smtk::view::Configuration::Component&)
  : m_icon(pqEyeball_svg)
  , m_iconClosed(pqEyeballClosed_svg)
  , m_parent(&parent)
{
  // Reset eyeball icons when the active view changes:
  pqActiveObjects& act(pqActiveObjects::instance());
  QObject::connect(&act, SIGNAL(viewChanged(pqView*)), this, SLOT(activeViewChanged(pqView*)));
  // Now call immediately, since in at least some circumstances, a view may already be active.
  if (this->phraseModel() && this->phraseModel()->root())
  {
    this->activeViewChanged(act.activeView());
  }
}

VisibilityBadge::~VisibilityBadge()
{
  m_parent = nullptr;
}

bool VisibilityBadge::appliesToPhrase(const DescriptivePhrase* phrase) const
{
  return !!vizQuery(
    Query::DISPLAYABLE, -1, phrase->content(), const_cast<VisibilityBadge*>(this)->m_visibleThings);
}

bool VisibilityBadge::phraseVisibility(const DescriptivePhrase* phrase) const
{
  return !!vizQuery(
    Query::GET_VALUE, -1, phrase->content(), const_cast<VisibilityBadge*>(this)->m_visibleThings);
}

void VisibilityBadge::setPhraseVisibility(const DescriptivePhrase* phrase, int val)
{
  vizQuery(Query::SET_VALUE, !!val ? 1 : 0, phrase->content(), this->m_visibleThings);
}

std::string VisibilityBadge::icon(
  const DescriptivePhrase* phrase, const std::array<float, 4>& /*background*/) const
{
  if (this->phraseVisibility(phrase))
    return m_icon;
  return m_iconClosed;
}

void VisibilityBadge::action(const DescriptivePhrase* phrase)
{
  int newVal = !this->phraseVisibility(phrase) ? 1 : 0;

  vizQuery(Query::SET_VALUE, newVal, phrase->content(), this->m_visibleThings);
  auto model = phrase->phraseModel();
  if (model)
  {
    model->triggerDataChanged();
  }
}

void VisibilityBadge::activeViewChanged(pqView* view)
{
  // Disconnect old representations, clear local visibility map.
  QObject::disconnect(this, SLOT(componentVisibilityChanged(smtk::resource::ComponentPtr, bool)));
  this->m_visibleThings.clear();
  // Connect new representations, initialize visibility map..
  if (view)
  {
    foreach (pqRepresentation* rep, view->getRepresentations())
    {
      this->representationAddedToActiveView(rep);
    }
    QObject::connect(view, SIGNAL(representationAdded(pqRepresentation*)), this,
      SLOT(representationAddedToActiveView(pqRepresentation*)));
    QObject::connect(view, SIGNAL(representationRemoved(pqRepresentation*)), this,
      SLOT(representationRemovedFromActiveView(pqRepresentation*)));
  }
  auto rsrcPhrases = this->phraseModel()->root()->subphrases();
  auto behavior = pqSMTKBehavior::instance();
  for (const auto& rsrcPhrase : rsrcPhrases)
  {
    auto rsrc = rsrcPhrase->relatedResource();
    if (!rsrc)
    {
      continue;
    }
    auto pvr = behavior->getPVResource(rsrc);
    auto rep = pvr ? pvr->getRepresentation(view) : nullptr;
    // TODO: At a minimum, we can update the representation's visibility now
    //       since if rep is null it is invisible and if not null, we can ask
    //       for its visibility.
    if (rep)
    {
      this->m_visibleThings[rsrc->id()] = rep->isVisible() ? 1 : 0;
      auto thingy = rep->getProxy()->GetClientSideObject();
      auto thingy2 = vtkCompositeRepresentation::SafeDownCast(thingy);
      auto srvrep = vtkSMTKResourceRepresentation::SafeDownCast(
        thingy2 ? thingy2->GetActiveRepresentation() : nullptr);
      if (srvrep)
      {
        // TODO: This assumes we are running in built-in mode. Remove the need for me.
        srvrep->GetEntityVisibilities(this->m_visibleThings);
      }
    }
    else
    {
      // This is a sign that things are going poorly.
      // The representation should already have been created either when
      // the view was created or the resource loaded.
      this->m_visibleThings[rsrc->id()] = behavior->createRepresentation(pvr, view) ? 1 : 0;
    }
  }
  // Indicate to the Qt model that it needs to refresh every row,
  // since visibility may be altered on each one:
  this->phraseModel()->triggerDataChanged();
}

void VisibilityBadge::representationAddedToActiveView(pqRepresentation* rep)
{
  auto modelRep = dynamic_cast<pqSMTKResourceRepresentation*>(rep);
  if (modelRep)
  {
    QObject::connect(modelRep,
      SIGNAL(componentVisibilityChanged(smtk::resource::ComponentPtr, bool)), this,
      SLOT(componentVisibilityChanged(smtk::resource::ComponentPtr, bool)));
  }
}

void VisibilityBadge::representationRemovedFromActiveView(pqRepresentation* rep)
{
  auto modelRep = dynamic_cast<pqSMTKResourceRepresentation*>(rep);
  if (modelRep)
  {
    QObject::disconnect(modelRep,
      SIGNAL(componentVisibilityChanged(smtk::resource::ComponentPtr, bool)), this,
      SLOT(componentVisibilityChanged(smtk::resource::ComponentPtr, bool)));
  }
}

void VisibilityBadge::componentVisibilityChanged(smtk::resource::ComponentPtr comp, bool visible)
{
  // The visibilty should change for every row displaying the same \a ent:
  this->m_visibleThings[comp->id()] = visible;
  this->phraseModel()->triggerDataChangedFor(comp);
}
}
}
}
}