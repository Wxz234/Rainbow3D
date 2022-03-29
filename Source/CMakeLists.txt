add_library(imgui STATIC 
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_tables.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_widgets.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/backends/imgui_impl_win32.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/backends/imgui_impl_dx11.cpp
)
target_include_directories(imgui PUBLIC ${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87 ${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/backends)

add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Log.cpp"
	"Vector.cpp"
	"Mesh.cpp"
	"Device.cpp"
	"Window.cpp"
	"Model.cpp"
)

target_compile_definitions(Rainbow3D PUBLIC UNICODE)
target_link_libraries(Rainbow3D PUBLIC imgui d3d11)