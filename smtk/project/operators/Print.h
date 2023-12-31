//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#ifndef smtk_project_Print_h
#define smtk_project_Print_h

#include "smtk/project/Operation.h"

namespace smtk
{
namespace project
{

/**\brief Lists project contents.

  Writes a list of project contents to the operation's log.
  */
class SMTKCORE_EXPORT Print : public smtk::project::Operation
{
public:
  smtkTypeMacro(smtk::project::Print);
  smtkCreateMacro(Print);
  smtkSharedFromThisMacro(smtk::operation::Operation);

protected:
  Result operateInternal() override;
  const char* xmlDescription() const override;
};
} // namespace project
} // namespace smtk

#endif
