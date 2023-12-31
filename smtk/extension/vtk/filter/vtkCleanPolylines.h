//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
// .NAME vtkCleanPolylines - filter removes polylines below threshold length
// .SECTION Description
// The filter removes polyline below threshold length.  Before analysis,
// duplicate points are merged, and then any input lines are stripped.  Then,
// before discarding any polylines, the polylines are appended to adjacent
// polylines where possible.

#ifndef smtk_vtk_CleanPolylines_h
#define smtk_vtk_CleanPolylines_h

#include "smtk/extension/vtk/filter/vtkSMTKFilterExtModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class VTKSMTKFILTEREXT_EXPORT vtkCleanPolylines : public vtkPolyDataAlgorithm
{
public:
  static vtkCleanPolylines* New();
  vtkTypeMacro(vtkCleanPolylines, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkCleanPolylines(const vtkCleanPolylines&) = delete;
  vtkCleanPolylines& operator=(const vtkCleanPolylines&) = delete;

  vtkSetClampMacro(MinimumLineLength, double, 0, VTK_FLOAT_MAX);
  vtkGetMacro(MinimumLineLength, double);

  // Description:
  // If "on", the average line length is multiplied by the MinimumLineLength
  // to determine the line test length
  vtkBooleanMacro(UseRelativeLineLength, bool);
  vtkSetMacro(UseRelativeLineLength, bool);
  vtkGetMacro(UseRelativeLineLength, bool);

protected:
  vtkCleanPolylines();

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  void TraverseLine(
    vtkIdType startPid,
    vtkIdType startCellId,
    vtkPolyData* input,
    unsigned char* marks,
    vtkIdList* ids,
    double* length,
    vtkIdType* lastLineId);
  void StripLines(vtkPolyData* input, vtkPolyData* result, vtkDoubleArray* lengths);
  void RemoveNonManifoldFeatures(
    vtkPolyData* input,
    vtkDoubleArray* lengths,
    vtkPolyData* result,
    vtkDoubleArray* newLengths);
  void TraversePolyLine(
    vtkIdType startPid,
    vtkIdType startCellId,
    vtkPolyData* input,
    vtkDoubleArray* lengths,
    unsigned char* marks,
    vtkIdList* ids,
    double* length);

private:
  double MinimumLineLength;
  bool UseRelativeLineLength;
};

#endif
