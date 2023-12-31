//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
// .NAME vtkPolygonContourOperation - convert contours to polygon edge(s)
// .SECTION Description
// Operation to convert the line cells in the input polygon to multiple edge(s)
// in a polygon model.

#ifndef smtk_polygon_vtkPolygonContourOperation_h
#define smtk_polygon_vtkPolygonContourOperation_h

#include "smtk/extension/vtk/operators/vtkSMTKOperation.h"
#include "smtk/session/polygon/vtk/vtkPolygonOperationsExtModule.h"

class vtkPolyData;

class VTKPOLYGONOPERATIONSEXT_EXPORT vtkPolygonContourOperation : public vtkSMTKOperation
{
public:
  static vtkPolygonContourOperation* New();
  vtkTypeMacro(vtkPolygonContourOperation, vtkSMTKOperation);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkPolygonContourOperation(const vtkPolygonContourOperation&) = delete;
  vtkPolygonContourOperation& operator=(const vtkPolygonContourOperation&) = delete;

  //Description:
  //Convert the passed-in polydata into polygon edge(s)
  bool AbleToOperate() override;
  smtk::operation::Operation::Result Operate() override;

  //Description:
  //Get/Set the Contour Input
  vtkGetObjectMacro(ContourInput, vtkPolyData);
  void SetContourInput(vtkPolyData* data);

  // Description:
  // The bounds of the image where the contours are created from
  vtkSetVector6Macro(ImageBounds, double);
  vtkGetVector6Macro(ImageBounds, double);

protected:
  vtkPolygonContourOperation();
  ~vtkPolygonContourOperation() override;

  vtkPolyData* ContourInput;
  double ImageBounds[6];
};

#endif
