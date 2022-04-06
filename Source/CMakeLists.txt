add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Log.cpp"
	"Vector.cpp"
	"Device.cpp"
	"Window.cpp"
	"Model.cpp"
)

target_compile_definitions(Rainbow3D PUBLIC UNICODE)
target_link_libraries(Rainbow3D PRIVATE d3d11 imgui nlohmann_json::nlohmann_json)