//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#ifndef smtk_model_Model_h
#define smtk_model_Model_h

#include "smtk/model/EntityRef.h"

#include "smtk/model/CellEntity.h"
#include "smtk/model/Group.h"

namespace smtk
{
namespace model
{

class Model;
typedef std::vector<CellEntity> CellEntities;
typedef std::vector<Model> Models;

/**\brief A entityref subclass that provides methods specific to models.
  *
  */
class SMTKCORE_EXPORT Model : public EntityRef
{
public:
  SMTK_ENTITYREF_CLASS(Model, EntityRef, isModel);

  ModelGeometryStyle geometryStyle() const;

  void setEmbeddingDimension(int dim);

  EntityRef parent() const;
  SessionRef session() const;
  void setSession(const SessionRef& sess);

  bool isModified() const;
  void setIsModified(bool isModified);

  CellEntities cells() const;
  Groups groups() const;
  Models submodels() const;
  AuxiliaryGeometries auxiliaryGeometry() const;

  template<typename T>
  T cellsAs() const;
  template<typename T>
  T groupsAs() const;
  template<typename T>
  T submodelsAs() const;
  template<typename T>
  void appendCells(T& container) const;

  Model& addCell(const CellEntity& c, bool checkExists = true);
  Model& removeCell(const CellEntity& c);
  template<typename T>
  Model& addCells(const T& container, bool checkExists = true);
  template<typename T>
  Model& removeCells(const T& container);

  Model& addGroup(const Group& g);
  Model& removeGroup(const Group& g);
  template<typename T>
  Model& addGroups(const T& container);
  template<typename T>
  Model& removeGroups(const T& container);

  Model& addSubmodel(const Model& m);
  Model& removeSubmodel(const Model& m);
  template<typename T>
  Model& addSubmodels(const T& container);
  template<typename T>
  Model& removeSubmodels(const T& container);

  Model& addAuxiliaryGeometry(const AuxiliaryGeometry& ag);
  Model& removeAuxiliaryGeometry(const AuxiliaryGeometry& ag);
  template<typename T>
  Model& addAuxiliaryGeometries(const T& container);
  template<typename T>
  Model& removeAuxiliaryGeometries(const T& container);

  void assignDefaultNames();

  EntityRefs entitiesWithTessellation() const;
};

/// Return the top-level (free) cells of this model in a container of the template type.
template<typename T>
T Model::cellsAs() const
{
  // TODO: This could be done more efficiently without a copy.
  CellEntities tmp = this->cells();
  T result;
  for (CellEntities::iterator it = tmp.begin(); it != tmp.end(); ++it)
  {
    typename T::value_type entry(*it);
    if (entry.isValid())
      result.insert(result.end(), *it);
  }
  return result;
}

/// Return the top-level (free) groups of this model in a container of the template type.
template<typename T>
T Model::groupsAs() const
{
  // TODO: This could be done more efficiently without a copy.
  Groups tmp = this->groups();
  T result;
  for (Groups::iterator it = tmp.begin(); it != tmp.end(); ++it)
  {
    typename T::value_type entry(*it);
    if (entry.isValid())
      result.insert(result.end(), *it);
  }
  return result;
}

/// Return the child models of this model in a container of the template type.
template<typename T>
T Model::submodelsAs() const
{
  // TODO: This could be done more efficiently without a copy.
  Models tmp = this->submodels();
  T result;
  for (Models::iterator it = tmp.begin(); it != tmp.end(); ++it)
  {
    typename T::value_type entry(*it);
    if (entry.isValid())
      result.insert(result.end(), *it);
  }
  return result;
}

/// Append free cells of this model to the given \a container. Only valid cells are inserted.
template<typename T>
void Model::appendCells(T& container) const
{
  // TODO: This could be done more efficiently without a copy.
  CellEntities tmp = this->cells();
  for (CellEntities::iterator it = tmp.begin(); it != tmp.end(); ++it)
  {
    typename T::value_type entry(*it);
    if (entry.isValid())
      container.insert(container.end(), *it);
  }
}

/// Add all the free cells in \a container to this model.
///
/// If \a checkExistence is true, then cells will only be
/// added if the model does not already reference them.
/// See addCell() for more information.
template<typename T>
Model& Model::addCells(const T& container, bool checkExistence)
{
  this->embedEntities(container, checkExistence);
  return *this;
}

/// Remove all the free cells in \a container from this model.
template<typename T>
Model& Model::removeCells(const T& container)
{
  this->unembedEntities(container);
  return *this;
}

/// Add all the groups in \a container to this model.
template<typename T>
Model& Model::addGroups(const T& container)
{
  for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    this->addGroup(*it);
  }
  return *this;
}

/// Add all the groups in \a container to this model.
template<typename T>
Model& Model::removeGroups(const T& container)
{
  for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    this->removeGroup(*it);
  }
  return *this;
}

/// Add all the models in \a container as submodels to this model.
template<typename T>
Model& Model::addSubmodels(const T& container)
{
  for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    this->addSubmodel(*it);
  }
  return *this;
}

/// Remove all the models in \a container from this model (if they are submodels).
template<typename T>
Model& Model::removeSubmodels(const T& container)
{
  for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    this->removeSubmodel(*it);
  }
  return *this;
}

/// Add all the auxiliary geometry entities in \a container to this model.
template<typename T>
Model& Model::addAuxiliaryGeometries(const T& container)
{
  for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    this->addAuxiliaryGeometry(*it);
  }
  return *this;
}

/// Remove all the auxiliary geometry entities in \a container from this model.
template<typename T>
Model& Model::removeAuxiliaryGeometries(const T& container)
{
  for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    this->removeAuxiliaryGeometry(*it);
  }
  return *this;
}

} // namespace model
} // namespace smtk

#endif // smtk_model_Model_h
