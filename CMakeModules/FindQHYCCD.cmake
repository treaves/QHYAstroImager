# - Try to find QHYCCD.
# Once executed, this module will define:
# Variables defined by this module:
#  QHYCCD_FOUND        - system has QHYCCD
#  QHYCCD_INCLUDE_DIR  - the QHYCCD include directory (cached)
#  QHYCCD_INCLUDE_DIRS - the QHYCCD include directories
#                         (identical to QHYCCD_INCLUDE_DIR)
#  QHYCCD_LIBRARY      - the QHYCCD library (cached)
#  QHYCCD_LIBRARIES    - the QHYCCD libraries
#                         (identical to QHYCCD_LIBRARY)
#
# This module will use the following enviornmental variable
# when searching for QHYCCD:
#  QHYCCD_ROOT_DIR     - QHYCCD root directory
#

#
#  Copyright © 2021 Timothy Reaves <treaves@silverfieldstech.com>
#
#

if(NOT QHYCCD_FOUND)

   find_path(QHYCCD_INCLUDE_DIR qhyccd.h
      HINTS $ENV{QHYCCD_ROOT_DIR}
      PATH_SUFFIXES include include/qhyccd)
   find_library(QHYCCD_LIBRARY qhyccd
      HINTS $ENV{QHYCCD_ROOT_DIR}
      PATH_SUFFIXES lib)

   mark_as_advanced(QHYCCD_INCLUDE_DIR QHYCCD_LIBRARY)

   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(QHYCCD DEFAULT_MSG
      QHYCCD_LIBRARY QHYCCD_INCLUDE_DIR)

   set(QHYCCD_INCLUDE_DIRS ${QHYCCD_INCLUDE_DIR})
   set(QHYCCD_LIBRARIES ${QHYCCD_LIBRARY})

endif(NOT QHYCCD_FOUND)
