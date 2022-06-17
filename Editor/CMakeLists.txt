add_executable(RainbowEditor WIN32 
	Editor.cpp
)
target_link_libraries(RainbowEditor PRIVATE RainbowCore RainbowShader RainbowRuntime)
target_compile_definitions(RainbowEditor PRIVATE UNICODE _UNICODE)
target_include_directories(RainbowEditor PRIVATE "${CMAKE_SOURCE_DIR}/Include")
target_link_libraries(RainbowEditor PRIVATE d3d12 dxgi dxguid d3dcompiler)