set(classes
  pqSMTKProjectAutoStart
  pqSMTKProjectBrowser
  pqSMTKProjectMenu
  pqSMTKProjectPanel
  Registrar
)

smtk_encode_file("${CMAKE_CURRENT_SOURCE_DIR}/ProjectPanelConfiguration.json"
  TYPE "_json"
  HEADER_OUTPUT rpJsonHeader)

set(CMAKE_AUTOMOC 1)
set(CMAKE_AUTOUIC 1)

vtk_module_add_module(smtkPQProjectExt
  CLASSES ${classes}
  HEADERS ${rpJsonHeader}
  HEADERS_SUBDIR "smtk/extension/paraview/project"
)
vtk_module_link(smtkPQProjectExt
  PUBLIC
    smtkCore
    smtkQtExt
    Qt5::Widgets
)
vtk_module_definitions(smtkPQProjectExt PRIVATE QT_NO_KEYWORDS)

target_link_libraries(smtkPQProjectExt
  PUBLIC
    ParaView::pqApplicationComponents
    smtkCore
    smtkPQComponentsExt
    smtkPVServerExt
)