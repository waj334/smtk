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
#ifndef smtk_common_PathsHelperWindows_h
#define smtk_common_PathsHelperWindows_h

#include "smtk/CoreExports.h"

#include <set>
#include <string>

namespace smtk
{
namespace common
{

class SMTKCORE_EXPORT PathsHelperWindows
{
public:
  PathsHelperWindows();

  static void AddSplitPaths(std::set<std::string>& splitPaths, const std::string& envVar);
};

} // namespace common
} // namespace smtk

#endif // smtk_common_PathsHelperWindows_h
