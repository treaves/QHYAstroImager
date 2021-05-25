# src/main

# ######################################################################################################################
# ##########                                        Process Resources                                         ##########
set(RESOURCES_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/resources")
set(RESOURCES "${RESOURCES_DIRECTORY}/resources.qrc")

# ######################################################################################################################
# ##########                                        Add Subdirectories                                        ##########
add_subdirectory(cpp/lib)
add_subdirectory(cpp/gui)
