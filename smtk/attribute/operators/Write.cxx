//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#include "smtk/attribute/operators/Write.h"

#include "smtk/attribute/Write_xml.h"

#include "smtk/attribute/Attribute.h"
#include "smtk/attribute/FileItem.h"
#include "smtk/attribute/IntItem.h"
#include "smtk/attribute/Resource.h"
#include "smtk/attribute/ResourceItem.h"

#include "smtk/attribute/json/jsonResource.h"

#include "smtk/common/Paths.h"

#include "smtk/io/Logger.h"

SMTK_THIRDPARTY_PRE_INCLUDE
//force to use filesystem version 3
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

#include "nlohmann/json.hpp"
SMTK_THIRDPARTY_POST_INCLUDE

#include <fstream>

namespace smtk
{
namespace attribute
{

Write::Result Write::operateInternal()
{
  auto resourceItem = this->parameters()->associations();

  smtk::attribute::Resource::Ptr resource =
    std::dynamic_pointer_cast<smtk::attribute::Resource>(resourceItem->objectValue());

  // Serialize resource into a set of JSON records:
  nlohmann::json j;
  smtk::attribute::to_json(j, resource);

  if (j.is_null())
  {
    return this->createResult(smtk::operation::Operation::Outcome::FAILED);
  }

  // Ensure the resource suffix is .smtk so readers can subsequently read the
  // file.
  std::string filename = resource->location();
  if (smtk::common::Paths::extension(resource->location()) != ".smtk")
  {
    filename = smtk::common::Paths::directory(filename) + "/" +
      smtk::common::Paths::stem(filename) + ".smtk";

    // If a file already exists with this name, append a distinguishing string
    // to the name.
    if (boost::filesystem::exists(filename.c_str()))
    {
      filename = smtk::common::Paths::directory(filename) + "/" +
        smtk::common::Paths::stem(filename) + "_att.smtk";
    }

    resource->setLocation(filename);
  }

  {
    std::ofstream file(resource->location());
    if (!file.good())
    {
      smtkErrorMacro(log(), "Unable to open \"" << resource->location() << "\" for writing.");
      return this->createResult(smtk::operation::Operation::Outcome::FAILED);
    }
    file << j.dump(2);
    file.close();
  }

  return this->createResult(smtk::operation::Operation::Outcome::SUCCEEDED);
}

const char* Write::xmlDescription() const
{
  return Write_xml;
}

void Write::markModifiedResources(Write::Result&)
{
  auto resourceItem = this->parameters()->associations();
  for (auto rit = resourceItem->begin(); rit != resourceItem->end(); ++rit)
  {
    auto resource = std::dynamic_pointer_cast<smtk::resource::Resource>(*rit);

    // Set the resource as unmodified from its persistent (i.e. on-disk) state
    resource->setClean(true);
  }
}

bool write(const smtk::resource::ResourcePtr& resource)
{
  Write::Ptr write = Write::create();
  write->parameters()->associate(resource);
  Write::Result result = write->operate();
  return (result->findInt("outcome")->value() == static_cast<int>(Write::Outcome::SUCCEEDED));
}
}
}
