file(GLOB_RECURSE imgui
	"ThirdParty/imgui/imgui.cpp"
	"ThirdParty/imgui/imgui_demo.cpp"
	"ThirdParty/imgui/imgui_draw.cpp"
	"ThirdParty/imgui/imgui_impl_dx11.cpp"
	"ThirdParty/imgui/imgui_impl_win32.cpp"
	"ThirdParty/imgui/imgui_tables.cpp"
	"ThirdParty/imgui/imgui_widgets.cpp"
)

file(GLOB_RECURSE DirectXTex
	"ThirdParty/DirectXTex/DDSTextureLoader/DDSTextureLoader11.cpp"
	"ThirdParty/DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
	"ThirdParty/DirectXTex/WICTextureLoader/WICTextureLoader11.cpp"
	"ThirdParty/DirectXTex/WICTextureLoader/WICTextureLoader11.h"
)

file(GLOB_RECURSE json
	"ThirdParty/json/json.hpp"
)
add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Rainbow3D.h"
	"Core/Log/Log.cpp"
	"Platform/Window/Window.cpp"
	"Render/Graphics/Utility.cpp"
	${imgui}
	${DirectXTex}
	${json}
)

source_group("imgui" FILES ${imgui})
source_group("DirectXTex" FILES ${DirectXTex})
source_group("json" FILES ${json})

target_compile_definitions(Rainbow3D PRIVATE UNICODE _UNICODE)
target_include_directories(Rainbow3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Rainbow3D PRIVATE d3d11 dxgi dxguid d3dcompiler)