//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
// .NAME vtkSMTKArcRepresentation - Default representation for the arc widget
// .SECTION Description
// This class provides the default concrete representation for the
// vtkContourWidget. It works in conjunction with the
// vtkContourLineInterpolator and vtkPointPlacer. See vtkContourWidget
// for details.
// .SECTION See Also
// vtkOrientedGlyphContourRepresentation vtkContourRepresentation vtkContourWidget vtkPointPlacer
// vtkContourLineInterpolator

#ifndef smtk_vtk_SMTKArcRepresentation_h
#define smtk_vtk_SMTKArcRepresentation_h

#include "smtk/extension/vtk/widgets/vtkSMTKWidgetsExtModule.h" // For export macro

#include "vtkOrientedGlyphContourRepresentation.h"

class vtkProperty;
class vtkActor;
class vtkPolyDataMapper;
class vtkPolyData;
class vtkGlyph3D;
class vtkPoints;

class VTKSMTKWIDGETSEXT_EXPORT vtkSMTKArcRepresentation
  : public vtkOrientedGlyphContourRepresentation
{
public:
  // Description:
  // Instantiate this class.
  static vtkSMTKArcRepresentation* New();

  vtkSMTKArcRepresentation(const vtkSMTKArcRepresentation&) = delete;
  vtkSMTKArcRepresentation& operator=(const vtkSMTKArcRepresentation&) = delete;

  // Description:
  // Standard methods for instances of this class.
  vtkTypeMacro(vtkSMTKArcRepresentation, vtkOrientedGlyphContourRepresentation);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  // Description:
  // Controls whether the contour widget should
  // generate a point array that represents if the point
  // was modifed. If used remember to turn off during construction
  // of the initial points
  // Default is to set it to false.
  vtkSetMacro(LoggingEnabled, int);
  vtkGetMacro(LoggingEnabled, int);
  vtkBooleanMacro(LoggingEnabled, int);

  //needed to make sure selected nodes are highlighted properly
  int SetNthNodeSelected(int) override;
  int ToggleActiveNodeSelected() override;

  // Get the number of selected nodes
  virtual int GetNumberOfSelectedNodes();

  // Description:
  // Given a display position, activate a node. The closest
  // node within tolerance will be activated. If a node is
  // activated, 1 will be returned, otherwise 0 will be
  // returned.
  // Override vtkContourRepresentation to make sure that if there is
  // no point yet in the representation, don't do any point locating.
  int ActivateNode(double displayPos[2]) override;

  //overloaded for logging purposes
  int DeleteNthNode(int n) override;
  int SetActiveNodeToWorldPosition(double worldPos[3], double worldOrient[9]) override;
  int SetActiveNodeToWorldPosition(double worldPos[3]) override;
  int AddNodeOnContour(int X, int Y) override;
  int AddNodeAtDisplayPosition(int X, int Y) override;
  void StartWidgetInteraction(double startEventPos[2]) override;

  // Description:
  // A method that the widget and its representation use to
  // communicate with each other.
  // Re-implement to add initial 0-point checking
  int ComputeInteractionState(int X, int Y, int modified = 0) override;

  // Description:
  // Get the points in this contour as a vtkPolyData.
  vtkPolyData* GetContourRepresentationAsPolyData() override;

  //Description:
  // Get the flags for a given point
  // the flags represent if the point has been moved, inserted or deleted
  // A point can have all three flags or none of them
  int GetNodeModifiedFlags(int n);

  // Description:
  // Controls whether the contour widget can be moved to the edit mode.
  // Default is 1.
  vtkSetMacro(CanEdit, int);
  vtkGetMacro(CanEdit, int);
  vtkBooleanMacro(CanEdit, int);

  vtkSetMacro(PointSelectMode, int);
  vtkGetMacro(PointSelectMode, int);
  vtkBooleanMacro(PointSelectMode, int);

  void SetPointSelectCallBack(vtkCommand* cp); // takes ownership
  //Description:
  // Expose method in superClass
  void InitializeContour(vtkPolyData* poly, vtkIdList*) override { this->Initialize(poly); }

protected:
  vtkSMTKArcRepresentation();
  ~vtkSMTKArcRepresentation() override;

  void UpdateLines(int index) override;
  void BuildRepresentation() override;

  // Description:
  // Build a contour representation from externally supplied PolyData. This
  // is very useful when you use an external program to compute a set of
  // contour nodes, let's say based on image features. Subsequently, you want
  // to build and display a contour that runs through those points.
  // This method is protected and accessible only from
  // vtkContourWidget::Initialize( vtkPolyData * )

  //Note: While this method will only render the first line cell in the polydata
  //it will compute if the contour is closed based on this first cell number of points
  //versus the number of points in the polydata. So don't have any extra points
  void Initialize(vtkPolyData*) override;

  //support logging of point changes
  int LoggingEnabled;

  //suppor the ability to disable editing of the arc
  int CanEdit;

  int PointSelectMode;
  vtkCommand* PointSelectCallBack;

  class vtkInternalMap;
  vtkInternalMap* ModifiedPointMap;

  void UpdatePropertyMap(int index, int flags);

private:
  unsigned pointId;
};

#endif
