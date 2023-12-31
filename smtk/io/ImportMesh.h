//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#ifndef smtk_io_ImportMesh_h
#define smtk_io_ImportMesh_h

#include "smtk/CoreExports.h" // For SMTKCORE_EXPORT macro.
#include "smtk/PublicPointerDefs.h"

#include "smtk/io/mesh/MeshIO.h"

#include <string>
#include <vector>

namespace smtk
{
namespace io
{

/**\brief Import an entire SMTK mesh resource from a file, or just sub-sections
  *
  */
class SMTKCORE_EXPORT ImportMesh
{
public:
  ImportMesh();
  ~ImportMesh();

  ImportMesh& operator=(const ImportMesh&) = delete;
  ImportMesh(const ImportMesh&) = delete;

  static std::vector<smtk::io::mesh::MeshIOPtr>& SupportedIOTypes();

  static bool ExtensionIsSupported(const std::string& ext);

  //Load the domain sets from a moab data file as a new resource with the
  //given interface.
  smtk::mesh::ResourcePtr operator()(
    const std::string& filePath,
    const smtk::mesh::InterfacePtr& interface,
    std::string domainPropertyName = std::string()) const;
  bool operator()(
    const std::string& filePath,
    smtk::mesh::ResourcePtr resource,
    std::string domainPropertyName = std::string()) const;

  //Return the file format selected to read the file at <filePath>.
  smtk::io::mesh::Format fileFormat(const std::string& filePath);
};

SMTKCORE_EXPORT smtk::mesh::ResourcePtr importMesh(
  const std::string& filePath,
  const smtk::mesh::InterfacePtr& interface);
SMTKCORE_EXPORT smtk::mesh::ResourcePtr importMesh(
  const std::string& filePath,
  const smtk::mesh::InterfacePtr& interface,
  const std::string& domainPropertyName);
SMTKCORE_EXPORT bool importMesh(const std::string& filePath, smtk::mesh::ResourcePtr resource);
SMTKCORE_EXPORT bool importMesh(
  const std::string& filePath,
  smtk::mesh::ResourcePtr resource,
  const std::string& domainPropertyName);
SMTKCORE_EXPORT smtk::io::mesh::Format meshFileFormat(const std::string& filePath);
} // namespace io
} // namespace smtk

#endif
