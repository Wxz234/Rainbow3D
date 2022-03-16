add_library(Rainbow3D_JSON STATIC "PRIVATE/Parser.cpp" "PUBLIC/JSON/JSON.h")
target_include_directories(Rainbow3D_JSON PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/PUBLIC")
target_link_libraries(Rainbow3D_JSON PUBLIC Rainbow3D_Debug)