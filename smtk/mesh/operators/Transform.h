//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#ifndef smtk_mesh_Transform_h
#define smtk_mesh_Transform_h

#include "smtk/operation/XMLOperation.h"

namespace smtk
{
namespace mesh
{

/**\brief Transform a mesh.
  */
class SMTKCORE_EXPORT Transform : public smtk::operation::XMLOperation
{
public:
  smtkTypeMacro(smtk::mesh::Transform);
  smtkCreateMacro(Transform);
  smtkSharedFromThisMacro(smtk::operation::Operation);
  smtkSuperclassMacro(smtk::operation::XMLOperation);

protected:
  Result operateInternal() override;
  const char* xmlDescription() const override;
};

} //namespace mesh
} // namespace smtk

#endif // smtk_mesh_Transform_h
