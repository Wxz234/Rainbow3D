add_library(Rainbow3D_Debug STATIC "PRIVATE/Debug.cpp" "PUBLIC/Debug/Debug.h")
target_include_directories(Rainbow3D_Debug PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/PUBLIC")