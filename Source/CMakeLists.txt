file(GLOB_RECURSE imgui
	"ThirdParty/imgui/imgui.cpp"
	"ThirdParty/imgui/imgui_demo.cpp"
	"ThirdParty/imgui/imgui_draw.cpp"
	"ThirdParty/imgui/imgui_impl_dx11.cpp"
	"ThirdParty/imgui/imgui_impl_win32.cpp"
	"ThirdParty/imgui/imgui_tables.cpp"
	"ThirdParty/imgui/imgui_widgets.cpp"
)

add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Rainbow3D.h"
	"Core/Log/Log.cpp"
	"RObject/RObject.cpp"
	"Platform/Window/Window.cpp"
	"Render/Graphics/Device.cpp"
	"Render/Graphics/SwapChain.cpp"
	"Render/Graphics/GraphicsUtility.cpp"
	${imgui}
)

source_group("imgui" FILES ${imgui})

target_compile_definitions(Rainbow3D PRIVATE UNICODE _UNICODE)
target_include_directories(Rainbow3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Rainbow3D PRIVATE d3d11 dxgi dxguid d3dcompiler)
