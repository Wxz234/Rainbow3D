add_library(Rainbow3D_UI STATIC "PRIVATE/UI.cpp" "PUBLIC/UI/UI.h")
target_include_directories(Rainbow3D_UI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/PUBLIC")