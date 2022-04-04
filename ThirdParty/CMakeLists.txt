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

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)