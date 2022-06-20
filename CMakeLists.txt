cmake_minimum_required(VERSION 3.20)
set(CMAKE_SYSTEM_VERSION 10.0.22621.0)
set(CMAKE_CXX_STANDARD 20)
project(Rainbow3D)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("ThirdParty")
add_subdirectory("Core")
add_subdirectory("Shader")
add_subdirectory("Runtime")
add_subdirectory("Editor")