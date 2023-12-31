//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//=============================================================================

#ifndef smtk_mesh_moab_Writers_h
#define smtk_mesh_moab_Writers_h

#include "smtk/PublicPointerDefs.h"

namespace smtk
{
namespace mesh
{
namespace moab
{

//Write everything in a file into an existing resource.
bool write(const std::string& path, const smtk::mesh::ResourcePtr& c);

//Write all the domain sets in a file into an existing resource
bool write_domain(const std::string& path, const smtk::mesh::ResourcePtr& c);

//Write all the neumann sets in a file into an existing resource
bool write_neumann(const std::string& path, const smtk::mesh::ResourcePtr& c);

//Write all the dirichlet sets in a file into an existing resource
bool write_dirichlet(const std::string& path, const smtk::mesh::ResourcePtr& c);
} // namespace moab
} // namespace mesh
} // namespace smtk

#endif
