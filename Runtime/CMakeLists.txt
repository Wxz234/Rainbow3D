add_library(RainbowRuntime STATIC
	Runtime.cpp
	ModelObject.cpp
)

target_compile_definitions(RainbowRuntime PRIVATE UNICODE _UNICODE)
target_include_directories(RainbowRuntime PRIVATE "${CMAKE_SOURCE_DIR}/Include")