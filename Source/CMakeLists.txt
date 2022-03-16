add_library(imgui STATIC 
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_tables.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/imgui_widgets.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/backends/imgui_impl_win32.cpp
	${PROJECT_SOURCE_DIR}/ThirdParty/imgui-1.87/backends/imgui_impl_dx11.cpp
)

add_subdirectory("Debug")
add_subdirectory("JSON")
add_subdirectory("UI")
add_executable(Rainbow3D WIN32 "Rainbow3D.cpp")
target_include_directories(Rainbow3D PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_link_libraries(Rainbow3D PRIVATE 
	Rainbow3D_Debug 
	Rainbow3D_JSON 
	Rainbow3D_UI
)