set_source_files_properties(${CMAKE_SOURCE_DIR}/Shader/bindless_vs.hlsl PROPERTIES VS_SHADER_TYPE Vertex VS_SHADER_MODEL 6.5 VS_SHADER_ENTRYPOINT main)

add_executable(RainbowShader 
	Shader.cpp
	bindless_vs.hlsl
)

target_link_libraries(RainbowShader PRIVATE RainbowCore)
target_compile_definitions(RainbowShader PRIVATE UNICODE _UNICODE)
target_include_directories(RainbowShader PRIVATE "${CMAKE_SOURCE_DIR}/Include")
