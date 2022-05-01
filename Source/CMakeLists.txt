add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Rainbow3D.h"
	"Core/Log/Log.cpp"
	"Platform/Window/Window.cpp"
	"Render/Device/GraphicsDevice.cpp"
	"Render/Renderer/Renderer.cpp"
	"Render/Renderer/MeshRenderer.cpp"
	"Render/Texture/Texture.cpp"
	"Render/RenderTarget/GBuffer.cpp"
	"Runtime/Model/Model.cpp"
	"Runtime/Model/GLTF.cpp"
	"Runtime/Mesh/Mesh.cpp"
	"Runtime/Scene/Scene.cpp"
	"Runtime/Camera/Camera.cpp"
)

target_compile_definitions(Rainbow3D PRIVATE UNICODE)
target_include_directories(Rainbow3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Rainbow3D PRIVATE d3d11 dxgi d3dcompiler)
