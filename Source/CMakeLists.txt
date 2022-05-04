add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Rainbow3D.h"
	"Core/Log/Log.cpp"
	"Platform/Window/Window.cpp"
	"Render/RHI/RHI.cpp"
)

target_compile_definitions(Rainbow3D PRIVATE UNICODE)
target_include_directories(Rainbow3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Rainbow3D PRIVATE d3d11 dxgi dxguid d3dcompiler)
