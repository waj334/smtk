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
// .NAME GridInfo.h - abstract class to get grid information.
// .SECTION Description
// Abstract class to get grid information. Note that we don't assume that
// the whole analysis grid is stored. CMB will implement a version
// of this to get the grid information into SMTK from CMB without
// introducing a dependency on CMB.
// .SECTION See Also

#ifndef __smtk_model_GridInfo_h
#define __smtk_model_GridInfo_h

#include "smtk/SMTKCoreExports.h"
#include "smtk/util/SystemConfig.h"

#include <string>
#include <vector>
#include <utility>

namespace smtk
{
  namespace util
  {
    class Logger;
  }

  namespace model
  {
    class SMTKCORE_EXPORT GridInfo
    {
    public:
      /// Return type for all public methods
      enum ApiReturnType
      {
        OK = 0,
        MODEL_ENTITY_NOT_FOUND,  // specified model entity id not found
        GRID_ENTITY_NOT_FOUND,   // specified grid entity id not found
        IDENTIFIER_NOT_FOUND,    // for methods specific to MOAB grids
        NOT_AVAILABLE            // requested data not available/found
      };


      /// Return structure for all public methods
      struct ApiStatus
      {
        ApiReturnType       returnType;
        std::string         errorMessage;
        smtk::util::Logger *logger;
      };

      /// Point closure enum, for point accessor methods
      enum PointClosure
      {
        ALL_POINTS = 0,
        INTERIOR_POINTS,
        BOUNDARY_POINTS
      };

      /// Returns highest dimension elements/cells in the grid
      virtual int dimension(ApiStatus& status) const = 0;


      /// Returns analysis grid cells for specified model entity.
      //  Only returns native/canonical ids specified in the analysis grid
      //  i.e., does *not* return internal CMB grid entities.
      virtual std::vector<int>
      analysisGridCells(int modelEntityId, ApiStatus& status) = 0;


      /// Returns "grid items" for the geometry on the boundary of a model entity.
      //  Each grid item is a pair of integer values; the first integer is the id
      //  of a cell in the analysis grid, and the second integer is the index
      //  of a particular side of the cell, using the VTK numbering convention.
      virtual std::vector<std::pair<int, int> >
      boundaryItemsOf(int modelEntityId, ApiStatus& status) = 0;


      /// Returns "grid items" for the geometry of a model entity that is on
      //  the boundary of a next-higher-dimension model entity.
      //  Each grid item is a pair of integer values; the first integer is the id
      //  of a cell in the analysis grid, and the second integer is the index
      //  of a particular side of the cell, using the VTK numbering convention.
      //  If the specified model entity is on the boundary of multiple model
      //  entities (for example, a model face can be on the boundary of two model
      //  regions), the grid items are returned for only one bounded model
      //  entity. If the last argument (boundedModelEntity) is specified,
      //  then the grid items will be returned specifically for that model
      //  entity (that is, the analysis grid cells will be from that model
      //  entity).
      virtual std::vector<std::pair<int, int> >
      asBoundaryItems(int modelEntityId, int boundedModelEntityId,
                      ApiStatus& status) = 0;


      /// Returns the type of cell for the specified analysis grid cell id.
      //  The integer value is defined by the VTKCellType enum in vtkCellType.h
      virtual int cellType(int gridCellId, ApiStatus& status) = 0;


      /// Returns the grid point ids for a specified model entity id.
      //  The list can be filtered by the point closure enumeration.
      virtual std::vector<int>
      pointIds(int modelEntityId, PointClosure closure, ApiStatus& status) = 0;


      /// Returns the grid point ids for a specified grid cell id.
      //  The points are ordered using the VTK convention.
      virtual std::vector<int>
      cellPointIds(int gridCellId, ApiStatus& status) = 0;


      /// Returns the dimensional coordinates of a specified grid point id.
      virtual std::vector<double>
      pointLocation(int gridPointId, ApiStatus& status) = 0;


      /// Returns the classification id for the node or element set for a
      //  specified model entity. The function applies to MOAB-style grids.
      //  Note that, although MOAB uses integer ids, this method returns a
      //  std::string for generality.
      virtual std::string
      nodeElemSetClassification(int modelEntityId, ApiStatus& status) = 0;


      /// Returns the classification id for the mesh side set for a specified
      //  model entity. The function applies to MOAB-style grids.
      //  Note that, although MOAB uses integer ids, this method returns a
      //  std::string for generality.
      virtual std::string
      sideSetClassification(int modelEntityId, ApiStatus& status) = 0;


      GridInfo();
      virtual ~GridInfo();
    };
  }
}

#endif /* __smtk_model_GridInfo_h */
