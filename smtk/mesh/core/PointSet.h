//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#ifndef smtk_mesh_core_PointSet_h
#define smtk_mesh_core_PointSet_h

#include "smtk/CoreExports.h"
#include "smtk/PublicPointerDefs.h"

#include "smtk/mesh/core/Handle.h"
#include "smtk/mesh/core/QueryTypes.h"

namespace smtk
{
namespace mesh
{

//forward declare
class PointLocator;

class SMTKCORE_EXPORT PointSet
{
  friend SMTKCORE_EXPORT PointSet set_intersect(const PointSet& a, const PointSet& b);
  friend SMTKCORE_EXPORT PointSet set_difference(const PointSet& a, const PointSet& b);
  friend SMTKCORE_EXPORT PointSet set_union(const PointSet& a, const PointSet& b);
  friend SMTKCORE_EXPORT void for_each(const PointSet& a, PointForEach& filter);

public:
  PointSet(const smtk::mesh::ResourcePtr& parent, const smtk::mesh::HandleRange& points);
  PointSet(const smtk::mesh::ConstResourcePtr& parent, const smtk::mesh::HandleRange& points);

  //construct a PointSet that represents an arbitrary unknown subset of points
  //that are children of the handle via an explicit vector of point ids. While
  //this method is inefficient, it is useful for the python bindings where
  //<points> is converted to a list.
  PointSet(const smtk::mesh::ResourcePtr& parent, const std::vector<smtk::mesh::Handle>& points);

  //construct a PointSet that represents an arbitrary unknown subset of points
  //that are children of the handle via an explicit set of point ids. This
  //constructor is preferred over the variant that takes a std::vector.
  PointSet(const smtk::mesh::ResourcePtr& parent, const std::set<smtk::mesh::Handle>& points);

  //Copy Constructor required for rule of 3
  PointSet(const PointSet& other);

  //required to be in the cpp file as we hold a HandleRange
  ~PointSet();

  //Copy assignment operator required for rule of 3
  PointSet& operator=(const PointSet& other);
  bool operator==(const PointSet& other) const;
  bool operator!=(const PointSet& other) const;

  bool is_empty() const;
  std::size_t size() const;

  //Get the number of points in the array
  std::size_t numberOfPoints() const;

  //returns true if the point set contains a given point id
  bool contains(const smtk::mesh::Handle& pointId) const;

  //returns the index in this point set for the given point id
  std::size_t find(const smtk::mesh::Handle& pointId) const;

  //Get all the point coordinates and store them in the passed in std::vector
  bool get(std::vector<double>& xyz) const;

  //Set all the point coordinates to the values from the input std::vector
  bool set(const std::vector<double>& xyz) const;

  // Skipping the following:
  // double*, and float* they are not nicely wrapped so the length is unknown
  // std::vector<float>& skipped since python uses doubles not floats

  //Get all the point coordinates and store them in a pre-allocated buffer
  bool get(double* xyz) const;

  //Get all the point coordinates and store them in a pre-allocated buffer
  //Floats are not how we store the coordinates internally, so asking for
  //the coordinates in such a manner could cause data inaccuracies to appear
  //so generally this is only used if you fully understand the input domain
  bool get(float* xyz) const;

  //Get all the point coordinates and store them in a std::vector
  //Floats are not how we store the coordinates internally, so asking for
  //the coordinates in such a manner could cause data inaccuracies to appear
  //so generally this is only used if you fully understand the input domain
  bool get(std::vector<float>& xyz) const;

  //Set all the point coordinates from the pre-allocated buffer. The
  //buffer is required to have a length of size*3.
  bool set(const double* xyz) const;

  //Set all the point coordinates from the pre-allocated buffer. The
  //buffer is required to have a length of size*3.
  bool set(const float* xyz);

  //Set all the point coordinates from the pre-allocated buffer. The
  //buffer is required to have a length of size*3.
  bool set(const std::vector<float>& xyz);

  //get the underlying HandleRange that this PointSet represents
  const smtk::mesh::HandleRange& range() const { return m_points; }

  //get the underlying resource that this PointSet belongs to
  const smtk::mesh::ResourcePtr& resource() const;

private:
  smtk::mesh::ResourcePtr m_parent;
  smtk::mesh::HandleRange m_points;
};

//intersect two set of points, placing the results in the return points object.
//This uses a point id based comparison, so points that have duplicate
//coordinates but different id's are considered to not intersect
//Note: If the points come from different resources the result will
//always be empty
SMTKCORE_EXPORT PointSet set_intersect(const PointSet& a, const PointSet& b);

//subtract points b from a, placing the results in the return points object.
//This uses a point id based comparison, so points that have duplicate
//coordinates but different id's are considered to not intersect
//Note: If the points come from different resources the result will
//always be empty
SMTKCORE_EXPORT PointSet set_difference(const PointSet& a, const PointSet& b);

//union two set of points, placing the results in the return points object.
//This uses a point id based comparison, so points that have duplicate
//coordinates but different id's are considered to not intersect
//Note: If the points come from different resources the result will
//always be empty
SMTKCORE_EXPORT PointSet set_union(const PointSet& a, const PointSet& b);

//apply a for_each point operator on each point in a container.
SMTKCORE_EXPORT void for_each(const PointSet& a, PointForEach& filter);
} // namespace mesh
} // namespace smtk

#endif
