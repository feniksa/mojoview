# - Try to find indb8
# Once done this will define
#  INDB8_FOUND - System has LibXml2
#  INDB8_INCLUDE_DIRS - The LibXml2 include directories
#  INDB8_LIBRARIES - The libraries needed to use LibXml2
#  INDB8_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
pkg_check_modules(PC_INDB8 QUIET indb8)
set(INDB8_DEFINITIONS ${PC_INDB8_CFLAGS_OTHER})

find_path(INDB8_INCLUDE_DIR mojodb/db/MojDb.h
          HINTS ${PC_INDB8_INCLUDEDIR} ${PC_INDB8_INCLUDE_DIRS}
          PATH_SUFFIXES libxml2 )

find_library(INDB8_LIBRARY NAMES xml2 libxml2
             HINTS ${PC_INDB8_LIBDIR} ${PC_INDB8_LIBRARY_DIRS} )

set(INDB8_LIBRARIES ${INDB8_LIBRARY} )
set(INDB8_INCLUDE_DIRS ${INDB8_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set INDB8_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibXml2  DEFAULT_MSG
                                  INDB8_LIBRARY INDB8_INCLUDE_DIR)

mark_as_advanced(INDB8_INCLUDE_DIR INDB8_LIBRARY )
