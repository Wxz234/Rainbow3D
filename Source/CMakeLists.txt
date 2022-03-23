add_library(imgui STATIC 
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_tables.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_widgets.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/backends/imgui_impl_win32.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/backends/imgui_impl_dx11.cpp
)


add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Debug.cpp"
	"UI.cpp"
	"Mesh.h"
)
target_include_directories(Rainbow3D PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_compile_definitions(Rainbow3D PUBLIC UNICODE)