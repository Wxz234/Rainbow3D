add_library(imgui STATIC 
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/backends/imgui_impl_win32.cpp
	imgui/backends/imgui_impl_dx11.cpp
)
target_include_directories(imgui PRIVATE imgui imgui/backends)

#find_package(nlohmann_json 3.2.0 REQUIRED)
add_subdirectory(nlohmann_json)
find_package(nlohmann_json 3.2.0 REQUIRED)