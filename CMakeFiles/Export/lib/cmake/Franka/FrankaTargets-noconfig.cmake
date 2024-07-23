#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Franka::Franka" for configuration ""
set_property(TARGET Franka::Franka APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Franka::Franka PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Poco::Foundation;Poco::Net"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libfranka.so.0.8.0"
  IMPORTED_SONAME_NOCONFIG "libfranka.so.0.8"
  )

list(APPEND _IMPORT_CHECK_TARGETS Franka::Franka )
list(APPEND _IMPORT_CHECK_FILES_FOR_Franka::Franka "${_IMPORT_PREFIX}/lib/libfranka.so.0.8.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
