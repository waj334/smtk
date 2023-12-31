//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#ifndef smtk_extensions_vtk_source_SourceFromModel_h
#define smtk_extensions_vtk_source_SourceFromModel_h
#ifndef __VTK_WRAP__

#include "smtk/AutoInit.h"

#include "smtk/PublicPointerDefs.h"
#include "smtk/extension/vtk/source/SourceGenerator.h"
#include "smtk/extension/vtk/source/vtkSMTKSourceExtModule.h"

namespace smtk
{
namespace extension
{
namespace vtk
{
namespace source
{

/// A GeneratorType for creating vtkSmartPointer<vtkAlgorithm>s
/// using smtk::resource::Resources as the key. This class extends
/// smtk::extension::vtk::source::SourceGenerator.
class VTKSMTKSOURCEEXT_EXPORT SourceFromModel
  : public smtk::common::
      GeneratorType<smtk::resource::ResourcePtr, vtkSmartPointer<vtkAlgorithm>, SourceFromModel>
{
public:
  bool valid(const smtk::resource::ResourcePtr&) const override;

  vtkSmartPointer<vtkAlgorithm> operator()(const smtk::resource::ResourcePtr&) override;
};
} // namespace source
} // namespace vtk
} // namespace extension
} // namespace smtk

#endif // __VTK_WRAP__
#endif
