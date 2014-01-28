/*=========================================================================

Copyright (c) 1998-2012 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO
PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
=========================================================================*/


#include "smtk/attribute/Item.h"
#include "smtk/attribute/Attribute.h"
#include "smtk/attribute/ItemDefinition.h"
#include "smtk/attribute/GroupItem.h"
#include "smtk/attribute/RefItem.h"
#include "smtk/attribute/ValueItem.h"
#include <iostream>
using namespace smtk::attribute;
using namespace smtk;

//----------------------------------------------------------------------------
Item::Item(Attribute *owningAttribute, int itemPosition):
  m_attribute(owningAttribute), m_owningItem(NULL),
  m_position(itemPosition), m_isEnabled(true), m_definition()
{
}

//----------------------------------------------------------------------------
Item::Item(Item *inOwningItem, int itemPosition, int inSubGroupPosition):
  m_attribute(NULL), m_owningItem(inOwningItem),
  m_position(itemPosition), m_subGroupPosition(inSubGroupPosition),
  m_isEnabled(true), m_definition()
{
}

//----------------------------------------------------------------------------
Item::~Item()
{
}
//----------------------------------------------------------------------------
AttributePtr Item::attribute() const
{
  if (this->m_attribute)
    {
    return this->m_attribute->pointer();
    }
  if (this->m_owningItem)
    {
    return this->m_owningItem->attribute();
    }
  return AttributePtr();
}
//----------------------------------------------------------------------------
ItemPtr Item::pointer() const
{
  // We need to find the object that owns this item
  if (this->m_attribute)
    {
    return this->m_attribute->item(this->m_position);
    }
  if (this->m_owningItem)
    {
    GroupItem *gitem = dynamic_cast<GroupItem *>(this->m_owningItem);
    if (gitem)
      {
      return gitem->item(static_cast<size_t>(this->m_subGroupPosition),
                         static_cast<size_t>(this->m_position));
      }
    ValueItem *vitem = dynamic_cast<ValueItem *>(this->m_owningItem);
    if (vitem)
      {
      if(vitem->numberOfChildrenItems() > 0)
        {
        const std::map<std::string, smtk::attribute::ItemPtr> childrenItems =
          vitem->childrenItems();
        std::map<std::string, smtk::attribute::ItemPtr>::const_iterator it =
          childrenItems.find(this->name());
        if (it != childrenItems.end())
          {
          return it->second;
          }
        }
      else if(vitem->isExpression(static_cast<size_t>(this->m_position)))
        {
        // assume that this is owned by an expression
        return vitem->expressionReference(static_cast<size_t>(this->m_position));
        }
      else
        {
        std::cerr << "Cannot find owning item.\n";
        }
      }
    }
  return ItemPtr();
}
//----------------------------------------------------------------------------
std::string Item::name() const
{
  if (!this->m_definition)
    {
    return "";
    }
  return this->m_definition->name();
}
//----------------------------------------------------------------------------
std::string Item::label() const
{
  if (!this->m_definition)
    {
    return "";
    }
  return this->m_definition->label();
}
//----------------------------------------------------------------------------
bool Item::setDefinition(smtk::attribute::ConstItemDefinitionPtr def)
{
  if (this->m_definition)
    {
    return false;
    }
  this->m_definition = def;
  if (def && def->isOptional())
    {
    this->m_isEnabled = def->isEnabledByDefault();
    }
  return true;
}
//----------------------------------------------------------------------------
bool Item::isOptional() const
{
  return this->m_definition->isOptional();
}
//----------------------------------------------------------------------------=
bool Item::isEnabled() const
{
  return this->isOptional() ? this->m_isEnabled : true;
}
//----------------------------------------------------------------------------
bool Item::isMemberOf(const std::string &category) const
{
  return this->definition()->isMemberOf(category);
}
//----------------------------------------------------------------------------
bool Item::isMemberOf(const std::vector<std::string> &categories) const
{
  return this->definition()->isMemberOf(categories);
}
//----------------------------------------------------------------------------
void Item::reset() 
{
  if (this->m_definition && this->m_definition->isOptional())
    {
    this->m_isEnabled = this->m_definition->isEnabledByDefault();
    }
}
//----------------------------------------------------------------------------
std::string Item::type2String(Item::Type t)
{
  switch (t)
    {
    case ATTRIBUTE_REF:
      return "AttributeRef";
    case COLOR:
      return "Color";
    case DIRECTORY:
      return "Directory";
    case DOUBLE:
      return "Double";
    case FILE:
      return "File";
    case GROUP:
      return "Group";
    case INT:
      return "Int";
    case STRING:
      return "String";
    case VOID:
      return "Void";
    default:
      return "";
    }
  return "Error!";
}
//----------------------------------------------------------------------------
Item::Type Item::string2Type(const std::string &s)
{
  if (s == "AttributeRef")
    {
    return ATTRIBUTE_REF;
    }
  if (s == "Color")
    {
    return COLOR;
    }
  if (s == "Directory")
    {
    return DIRECTORY;
    }
  if (s == "Double")
    {
    return DOUBLE;
    }
  if (s == "File")
    {
    return FILE;
    }
  if (s == "Group")
    {
    return GROUP;
    }
  if (s == "Int")
    {
    return INT;
    }
  if (s == "String")
    {
    return STRING;
    }
  if (s == "Void")
    {
    return VOID;
    }
  return NUMBER_OF_TYPES;
}
//----------------------------------------------------------------------------
