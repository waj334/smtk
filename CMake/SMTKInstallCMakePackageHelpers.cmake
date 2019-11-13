function (_smtk_package_append_variables)
  set(_smtk_package_variables)
  foreach (var IN LISTS ARGN)
    if (NOT ${var})
      continue ()
    endif ()

    string(APPEND _smtk_package_variables
      "if (NOT DEFINED \"${var}\")
  set(\"${var}\" \"${${var}}\")
elseif (NOT ${var})
  set(\"${var}\" \"${${var}}\")
endif ()
")
  endforeach ()

  set(smtk_find_package_code
    "${smtk_find_package_code}${_smtk_package_variables}"
    PARENT_SCOPE)
endfunction ()

set(_smtk_packages
  Boost
  nlohmann_json
  pegtl
  MOAB
  Qt5
  ParaView
  VTK
  Remus
  ZeroMQ # for Remus
  pybind11)

# Per-package variable forwarding goes here.
set(Boost_find_package_vars
  Boost_INCLUDE_DIR)
set(ZeroMQ_find_package_vars
  ZeroMQ_INCLUDE_DIR
  ZeroMQ_LIBRARY)

set(smtk_find_package_code)
foreach (_smtk_package IN LISTS _smtk_packages)
  _smtk_package_append_variables(
    # Standard CMake `find_package` mechanisms.
    "${_smtk_package}_DIR"
    "${_smtk_package}_ROOT"

    # Per-package custom variables.
    ${${_smtk_package}_find_package_vars})
endforeach ()

file(GENERATE
  OUTPUT  "${smtk_cmake_build_dir}/smtk-find-package-helpers.cmake"
  CONTENT "${smtk_find_package_code}")