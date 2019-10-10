//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#include "jsonItemDefinition.h"
#include "smtk/PublicPointerDefs.h"
#include "smtk/attribute/ItemDefinition.h"

#include "nlohmann/json.hpp"
#include "smtk/CoreExports.h"

#include <string>

/**\brief Provide a way to serialize itemPtr
  */
namespace smtk
{
namespace attribute
{
SMTKCORE_EXPORT void to_json(
  nlohmann::json& j, const smtk::attribute::ItemDefinitionPtr& itemDefPtr)
{
  j = {
    { "Type", smtk::attribute::Item::type2String(itemDefPtr->type()) },
    { "Name", itemDefPtr->name() }, { "Version", itemDefPtr->version() },
  };
  if (itemDefPtr->label() != itemDefPtr->name())
  {
    j["Label"] = itemDefPtr->label();
  }
  if (itemDefPtr->isOptional())
  {
    j["Optional"] = true;
    j["isEnabledByDefault"] = itemDefPtr->isEnabledByDefault();
  }
  if (itemDefPtr->categoryCheckMode() == smtk::attribute::ItemDefinition::CategoryCheckMode::All)
  {
    j["categoryCheckMode"] = "All";
  }
  else
  {
    j["categoryCheckMode"] = "Any";
  }
  if (itemDefPtr->advanceLevel(0) || itemDefPtr->advanceLevel(1))
  {
    // OK - we have a non-zero advance level in either read or write
    // if they are both set the same use the AdvanceLevel xml attribute
    if (itemDefPtr->advanceLevel(0) == itemDefPtr->advanceLevel(1))
    {
      j["AdvanceLevel"] = itemDefPtr->advanceLevel(0);
    }
    else
    {
      if (itemDefPtr->advanceLevel(0))
      {
        j["AdvanceReadLevel"] = itemDefPtr->advanceLevel(0);
      }
      if (itemDefPtr->advanceLevel(1))
      {
        j["AdvanceWriteLevel"] = itemDefPtr->advanceLevel(1);
      }
    }
  }
  if (!itemDefPtr->localCategories().empty())
  {
    j["Categories"] = itemDefPtr->localCategories();
  }
  j["OkToInheritCategories"] = itemDefPtr->isOkToInherit();
  if (!itemDefPtr->briefDescription().empty())
  {
    j["BriefDescription"] = itemDefPtr->briefDescription();
  }
  if (!itemDefPtr->detailedDescription().empty())
  {
    j["DetailedDescription"] = itemDefPtr->detailedDescription();
  }
}

SMTKCORE_EXPORT void from_json(
  const nlohmann::json& j, smtk::attribute::ItemDefinitionPtr& itemDefPtr)
{
  // The caller should make sure that defPtr is valid since it's not default constructible
  if (!itemDefPtr.get())
  {
    return;
  }
  auto result = j.find("Label");
  if (result != j.end())
  {
    itemDefPtr->setLabel(*result);
  }
  result = j.find("Version");
  if (result != j.end())
  {
    itemDefPtr->setVersion(*result);
  }
  result = j.find("Optional");
  if (result != j.end())
  {
    itemDefPtr->setIsOptional(*result);
  }
  result = j.find("isEnabledByDefault");
  if (result != j.end())
  {
    itemDefPtr->setIsEnabledByDefault(*result);
  }
  result = j.find("categoryCheckMode");
  if (result != j.end())
  {
    if (*result == "All")
    {
      itemDefPtr->setCategoryCheckMode(smtk::attribute::ItemDefinition::CategoryCheckMode::All);
    }
    else
    {
      itemDefPtr->setCategoryCheckMode(smtk::attribute::ItemDefinition::CategoryCheckMode::Any);
    }
  }
  result = j.find("AdvanceLevel");
  if (result != j.end())
  {
    itemDefPtr->setAdvanceLevel(0, *result);
    itemDefPtr->setAdvanceLevel(1, *result);
  }
  else
  {
    auto val = j.find("AdvanceReadLevel");
    if (val != j.end())
    {
      itemDefPtr->setAdvanceLevel(0, *val);
    }
    val = j.find("AdvanceWriteLevel");
    if (val != j.end())
    {
      itemDefPtr->setAdvanceLevel(1, *val);
    }
  }

  result = j.find("BriefDescription");
  if (result != j.end())
  {
    itemDefPtr->setBriefDescription(*result);
  }
  result = j.find("DetailedDescription");
  if (result != j.end())
  {
    itemDefPtr->setDetailedDescription(*result);
  }
  auto categories = j.find("Categories");
  if (categories != j.end())
  {
    for (const auto& category : *categories)
    {
      itemDefPtr->addLocalCategory(category);
    }
  }
  auto okToInherit = j.find("OkToInheritCategories");
  if (okToInherit != j.end())
  {
    itemDefPtr->setIsOkToInherit(*okToInherit);
  }
}
}
}
