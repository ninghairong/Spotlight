#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "qhotkey" for configuration "RelWithDebInfo"
set_property(TARGET qhotkey APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(qhotkey PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/qhotkey.lib"
  )

list(APPEND _cmake_import_check_targets qhotkey )
list(APPEND _cmake_import_check_files_for_qhotkey "${_IMPORT_PREFIX}/lib/qhotkey.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
