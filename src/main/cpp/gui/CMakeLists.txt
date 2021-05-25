# src/main/cpp/gui

# This is needed only so that promoted widgets in Designer are found.
include_directories(${CMAKE_CURRENT_LIST_DIR}/ui)

# ######################################################################################################################
# ##########                                          Source Files                                            ##########
set(UIS ui/MainWindow.ui)

set(SOURCES
    main.cpp
    ui/MainWindow.cpp
)

set(HEADERS
    ui/MainWindow.hpp
)

# ######################################################################################################################
# ##########                                       Executable Creation                                        ##########
if(APPLE)
  set(CMAKE_MACOSX_BUNDLE 1)
  set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")

  set(MACOSX_BUNDLE_BUNDLE_NAME QUCU)
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.silverfieldstech.QHYAstroImager")
  set(MACOSX_BUNDLE_INFO_STRING "An application for QHYCCD cameras.")
  set(MACOSX_BUNDLE_ICON_FILE "Icon.icns")
  set(MACOSX_BUNDLE_LONG_VERSION_STRING "${${PROJECT}_VERSION}")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${${PROJECT}_MAJOR}.${${PROJECT}_MINOR}.${${PROJECT}_PATCH}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${${PROJECT}_VERSION}")

  set(ICON "${RESOURCES_DIRECTORY}/package/Mac/ezcap.icns")
  # put the icon into the Resources directory of the App bundle.
  set_source_files_properties(${ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

  add_executable(QHYAstroImager MACOSX_BUNDLE ${HEADERS} ${SOURCES} ${UIS} ${RESOURCES} ${ICON})
else()
  add_executable(QHYAstroImager ${HEADERS} ${SOURCES} ${UIS} ${RESOURCES})
endif()

target_link_libraries(
  QHYAstroImager
  PUBLIC Qt5::Core Qt5::Widgets
  PRIVATE qhyccd project_warnings project_options
)

target_compile_features(QHYAstroImager PUBLIC cxx_nullptr cxx_lambdas)

# ######################################################################################################################
# ##########                                   Install & Apple app support                                    ##########
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

install(
  TARGETS QHYAstroImager
  DESTINATION .
  COMPONENT Runtime
)

if(APPLE)
  # Get reference to deployqt
  get_target_property(uic_location Qt5::uic IMPORTED_LOCATION)
  get_filename_component(_dir ${uic_location} DIRECTORY)
  set(deployqt "${_dir}/macdeployqt")
  if(NOT EXISTS ${deployqt})
    message(FATAL_ERROR "Failed to locate deployqt executable: [${deployqt}]")
  endif()

  # Execute deployqt during package creation See https://doc.qt.io/qt-5/osx-deployment.html#macdeploy
  install(CODE "set(deployqt \"${deployqt}\")" COMPONENT Runtime)
  install(CODE [===[
   execute_process(COMMAND "${deployqt}" "${CMAKE_INSTALL_PREFIX}/ROMControl.app")
   ]===] COMPONENT Runtime
  )

  set(CPACK_GENERATOR "DragNDrop")
  include(CPack)
endif()

# ######################################################################################################################
# ##########                                         CMake debugging                                          ##########
# #################### Troubleshoot cmake variables
# ~~~
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
# #################### Troubleshoot cmake variables
# ~~~
