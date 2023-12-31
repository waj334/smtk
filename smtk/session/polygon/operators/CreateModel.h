//=============================================================================
// Copyright (c) Kitware, Inc.
// All rights reserved.
// See LICENSE.txt for details.
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notice for more information.
//=============================================================================
#ifndef smtk_session_polygon_CreateModel_h
#define smtk_session_polygon_CreateModel_h

#include "smtk/session/polygon/Operation.h"

namespace smtk
{
namespace session
{
namespace polygon
{

/**\brief Create a polygonal model made up of vertices, edges, and faces.
  *
  * The geometry in the model is all planar.
  * By default, points are assumed to lie in the x-y plane with an
  * origin of (0,0), but you may provide any base point and axes you prefer.
  *
  * Coordinates are discretized to integers; you must pass either a feature
  * size or a model scale to control how fine the approximation is.
  *
  * Each polygonal modeling session may have multiple models but no
  * geometric entities may be shared between them;
  * attempting to share points across different discretizations on different
  * projected planes would be error-prone at best.
  */
class SMTKPOLYGONSESSION_EXPORT CreateModel : public Operation
{
public:
  smtkTypeMacro(smtk::session::polygon::CreateModel);
  smtkCreateMacro(CreateModel);
  smtkSharedFromThisMacro(smtk::operation::Operation);
  smtkSuperclassMacro(Operation);

protected:
  Result operateInternal() override;
  const char* xmlDescription() const override;
};

} // namespace polygon
} //namespace session
} // namespace smtk

#endif // smtk_session_polygon_CreateModel_h
