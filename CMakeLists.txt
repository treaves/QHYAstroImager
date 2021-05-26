cmake_minimum_required(VERSION 3.15)

# ######################################################################################################################
# ##########                                    Project-related information                                   ##########
project(
  QHYAstroImager
  LANGUAGES CXX
  VERSION 1.0.0
)

# configure a header file to pass some of the CMake settings to the source code
configure_file("${PROJECT_SOURCE_DIR}/Config.h.in" "${PROJECT_BINARY_DIR}/Config.h")

# add the binary tree to the search path for include files so that we will find Config.h
include_directories("${PROJECT_BINARY_DIR}")

# ######################################################################################################################
# ##########                                           build add-ins.                                         ##########
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

# ######################################################################################################################
# ##########                                           Dependencies                                           ##########
list(
  APPEND
  CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMakeModules
  /usr/local/cmake_modules
)

find_package(
  Qt5 5.10
  COMPONENTS Core Test Widgets
  REQUIRED
)

find_package(CFITSIO REQUIRED)
find_package(QHYCCD REQUIRED)

# ######################################################################################################################
# ##########                                           Qt UI & moc                                            ##########
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(AUTOGEN_SOURCE_GROUP "Generated Files")

# ######################################################################################################################
# ##########                                        Add Subdirectories                                        ##########
add_subdirectory(src/main)

# ######################################################################################################################
# ##########                                              CPACK                                               ##########
# build a CPack driven installer package
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT}_MAJOR")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT}_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT}_PATCH}")
include(CPack)
