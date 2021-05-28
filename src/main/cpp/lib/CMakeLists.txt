# src/main/cpp/lib

# ######################################################################################################################
# ##########                                      Library Source Files                                        ##########
set(SOURCES QHYCCD.cpp QHYCamera.cpp)

set(HEADERS QHYCCD.hpp QHYCamera.hpp)

set(PRIVATE_SOURCE )

add_library(qhyccd STATIC ${SOURCES} ${HEADERS} ${PRIVATE_SOURCE})

target_link_libraries(
  qhyccd
  PUBLIC Qt5::Core Qt5::Widgets
  PRIVATE project_warnings project_options ${QHYCCD_LIBRARIES}
)
target_include_directories(qhyccd PUBLIC ${CMAKE_CURRENT_LIST_DIR} PRIVATE ${QHYCCD_INCLUDE_DIRS})
