//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#include "smtk/extension/paraview/appcomponents/pqSMTKDisplayAttributeOnLoadBehavior.h"

// SMTK
#include "smtk/extension/paraview/appcomponents/pqSMTKAttributePanel.h"
#include "smtk/extension/paraview/appcomponents/pqSMTKBehavior.h"
#include "smtk/extension/paraview/appcomponents/pqSMTKResource.h"
#include "smtk/extension/paraview/appcomponents/pqSMTKWrapper.h"

#include "smtk/extension/paraview/server/vtkSMSMTKWrapperProxy.h"

#include "smtk/view/Selection.h"

#include "smtk/resource/Resource.h"

#include "smtk/io/Logger.h"

// Client side
#include "pqCoreUtilities.h"
#include "pqLiveInsituManager.h"
#include "pqPipelineSource.h"
#include "pqSelectionManager.h"
#include "pqServerManagerModel.h"

// Qt
#include <QMainWindow>
#include <QTimer>
#include <QWidget>

using namespace smtk;

static pqSMTKDisplayAttributeOnLoadBehavior* g_displayOnLoad = nullptr;

pqSMTKDisplayAttributeOnLoadBehavior::pqSMTKDisplayAttributeOnLoadBehavior(QObject* parent)
  : Superclass(parent)
  , m_panel(nullptr)
  , m_attr(nullptr)
{
  if (!g_displayOnLoad)
  {
    g_displayOnLoad = this;
  }

  // Track server connects/disconnects
  auto rsrcBehavior = pqSMTKBehavior::instance();
  QObject::connect(rsrcBehavior, SIGNAL(addedManagerOnServer(vtkSMSMTKWrapperProxy*, pqServer*)),
    this, SLOT(observeResourcesOnServer(vtkSMSMTKWrapperProxy*, pqServer*)));
  QObject::connect(rsrcBehavior,
    SIGNAL(removingManagerFromServer(vtkSMSMTKWrapperProxy*, pqServer*)), this,
    SLOT(unobserveResourcesOnServer(vtkSMSMTKWrapperProxy*, pqServer*)));
}

pqSMTKDisplayAttributeOnLoadBehavior::~pqSMTKDisplayAttributeOnLoadBehavior()
{
  if (g_displayOnLoad == this)
  {
    g_displayOnLoad = nullptr;
  }
}

pqSMTKDisplayAttributeOnLoadBehavior* pqSMTKDisplayAttributeOnLoadBehavior::instance(
  QObject* parent)
{
  if (!g_displayOnLoad)
  {
    g_displayOnLoad = new pqSMTKDisplayAttributeOnLoadBehavior(parent);
  }

  return g_displayOnLoad;
}

void pqSMTKDisplayAttributeOnLoadBehavior::observeResourcesOnServer(
  vtkSMSMTKWrapperProxy* mgr, pqServer* server)
{
  (void)server;
  if (!mgr)
  {
    std::cerr << "pqSMTKDisplayAttributeOnLoadBehavior: no wrapper\n";
    return;
  }
  auto rsrcMgr = mgr->GetResourceManager();
  if (!rsrcMgr)
  {
    std::cerr << "pqSMTKDisplayAttributeOnLoadBehavior: no resource manager to observe\n";
    return;
  }

  int observerId = rsrcMgr->observers().insert(
    [this](smtk::resource::ResourcePtr rsrc, smtk::resource::EventType event) {
      this->handleResourceEvent(rsrc, event);
    },
    /* immediatelyNotify */ true);
  m_resourceManagerObservers[rsrcMgr] = observerId;
}

void pqSMTKDisplayAttributeOnLoadBehavior::unobserveResourcesOnServer(
  vtkSMSMTKWrapperProxy* mgr, pqServer* server)
{
  (void)server;
  if (!mgr)
  {
    return;
  }
  auto rsrcMgr = mgr->GetResourceManager();
  if (!rsrcMgr)
  {
    return;
  }

  auto entry = m_resourceManagerObservers.find(rsrcMgr);
  if (entry != m_resourceManagerObservers.end())
  {
    rsrcMgr->observers().erase(entry->second);
    m_resourceManagerObservers.erase(entry);
  }
}

void pqSMTKDisplayAttributeOnLoadBehavior::handleResourceEvent(
  smtk::resource::ResourcePtr rsrc, smtk::resource::EventType event)
{
  auto attr = std::dynamic_pointer_cast<smtk::attribute::Resource>(rsrc);
  if (attr)
  {
    // Find the attribute panel. For now, only deal with one;
    // it doesn't make sense to switch multiple panels to display
    // the same attribute anyway.
    pqSMTKAttributePanel* panel = nullptr;
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(pqCoreUtilities::mainWidget());
    foreach (pqSMTKAttributePanel* attrPanel, mainWindow->findChildren<pqSMTKAttributePanel*>())
    {
      panel = attrPanel;
      if (panel)
      {
        break;
      }
    }
    if (!panel)
    {
      return;
    }

    switch (event)
    {
      case smtk::resource::EventType::MODIFIED:
        // Do nothing.
        break;
      case smtk::resource::EventType::ADDED:
        // We want to just display things now, but at this point,
        // the attribute may exist but be empty because it is added
        // to the resource manager before the reader inserts data.
        // So, queue the slot to be invoked by a timer. Ugly!
        m_attr = attr;
        m_panel = panel;
        QTimer::singleShot(0, this, SLOT(displayAttribute()));
        break;
      case smtk::resource::EventType::REMOVED:
        // TODO: Find another attribute to display
        break;
    }
  }
}

void pqSMTKDisplayAttributeOnLoadBehavior::displayAttribute()
{
  if (m_panel && m_attr)
  {
    m_panel->displayResourceOnServer(m_attr);
    m_panel = nullptr;
    m_attr = nullptr;
  }
}