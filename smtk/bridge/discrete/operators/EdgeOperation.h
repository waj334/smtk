//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#ifndef __smtk_session_discrete_EdgeOperation_h
#define __smtk_session_discrete_EdgeOperation_h

#include "smtk/bridge/discrete/Operation.h"
#include "smtk/bridge/discrete/Resource.h"
#include "smtk/bridge/discrete/operation/vtkEdgeSplitOperation.h"
#include "smtk/bridge/discrete/operation/vtkMergeOperation.h"
#include "vtkNew.h"
#include <map>
#include <set>

class vtkDiscreteModelWrapper;
class vtkDiscreteModelVertex;
class vtkDiscreteModelEdge;

namespace smtk
{
namespace mesh
{
class MeshSet;
typedef std::set<smtk::mesh::MeshSet> MeshSets;
}
}

namespace smtk
{
namespace bridge
{
namespace discrete
{

class Session;

class SMTKDISCRETESESSION_EXPORT EdgeOperation : public Operation
{
public:
  smtkTypeMacro(smtk::bridge::discrete::EdgeOperation);
  smtkCreateMacro(EdgeOperation);
  smtkSharedFromThisMacro(smtk::operation::Operation);
  smtkSuperclassMacro(Operation);

  bool ableToOperate() override;

protected:
  EdgeOperation();
  Result operateInternal() override;
  const char* xmlDescription() const override;

  // some internal methods
  void getSelectedVertsAndEdges(smtk::bridge::discrete::Resource::Ptr& resource,
    std::map<smtk::common::UUID, vtkDiscreteModelVertex*>& selVTXs,
    std::map<smtk::common::UUID, std::pair<vtkDiscreteModelEdge*, std::set<int> > >& selArcs,
    const smtk::attribute::MeshSelectionItemPtr& inSelectionItem,
    smtk::bridge::discrete::SessionPtr opsession);
  bool convertSelectedEndNodes(const std::map<smtk::common::UUID, vtkDiscreteModelVertex*>& selVTXs,
    vtkDiscreteModelWrapper* modelWrapper, smtk::bridge::discrete::SessionPtr opsession,
    smtk::model::EntityRefArray& srcsRemoved, smtk::model::EntityRefArray& srcsModified,
    smtk::mesh::MeshSets& modifiedMeshes, vtkMergeOperation* mergOp);
  bool splitSelectedEdgeNodes(
    const std::map<smtk::common::UUID, std::pair<vtkDiscreteModelEdge*, std::set<int> > >& selArcs,
    vtkDiscreteModelWrapper* modelWrapper, smtk::bridge::discrete::SessionPtr opsession,
    smtk::model::EntityRefArray& srcsCreated, smtk::model::EntityRefArray& srcsModified,
    smtk::mesh::MeshSets& modifiedMeshes, vtkEdgeSplitOperation* splitOp);
  int convertToGlobalPointId(smtk::bridge::discrete::Resource::Ptr& resource, int localPid,
    vtkDiscreteModelEdge* cmbModelEdge);

  vtkNew<vtkEdgeSplitOperation> m_splitOp;
  vtkNew<vtkMergeOperation> m_mergeOp;
};

} // namespace discrete
} // namespace bridge
} // namespace smtk

#endif // __smtk_session_discrete_EdgeOperation_h
