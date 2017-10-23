# set up sources to build
set(resourceSrcs
  Component.cxx
  Resource.cxx
  SelectionManager.cxx
  Set.cxx
)

set(resourceHeaders
  Component.h
  Resource.h
  SelectionManager.h
  Set.h
)

if (SMTK_ENABLE_PYTHON_WRAPPING)
  add_subdirectory(pybind11)
endif()

#install the headers
smtk_public_headers(${resourceHeaders})

if (SMTK_ENABLE_TESTING)
  add_subdirectory(testing)
endif()