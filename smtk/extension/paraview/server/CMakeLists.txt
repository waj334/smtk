set(headers
  vtkSMTKRepresentationStyleGenerator.h
)

set(classes
  Registrar
  VTKSelectionResponderGroup

  vtkSMSMTKResourceRepresentationProxy
  vtkSMSMTKWrapperProxy
  vtkSMTKResource
  vtkSMTKResourceCreator
  vtkSMTKResourceGenerator
  vtkSMTKResourceImporter
  vtkSMTKResourceReader
  vtkSMTKResourceRepresentation
  vtkSMTKResourceSource
  vtkSMTKSettings
  vtkSMTKWrapper)

set(pvServerOps
  RespondToVTKSelection
)
foreach (operator ${pvServerOps})
  smtk_encode_file("${CMAKE_CURRENT_SOURCE_DIR}/${operator}.sbt"
  HEADER_OUTPUT headerName)
  list(APPEND classes ${operator})
  list(APPEND headers ${headerName})
endforeach()
# since generated headers are in this directory, we can add them without a custom target

vtk_module_add_module(smtkPVServerExt
  CLASSES ${classes}
  HEADERS ${headers}
  HEADERS_SUBDIR "smtk/extension/paraview/server")

if (ParaView_VERSION VERSION_LESS "5.10.0")
  vtk_module_definitions(smtkPVServerExt
    PRIVATE SMTK_PV_USE_CONFIG)
endif ()

vtk_module_link(smtkPVServerExt
  PUBLIC
    nlohmann_json)

paraview_add_server_manager_xmls(
  XMLS smconfig.xml)

if (SMTK_ENABLE_TESTING)
  add_subdirectory(testing)
endif ()
