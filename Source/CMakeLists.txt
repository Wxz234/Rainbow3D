add_executable(Rainbow3D WIN32
	"JSON/Parser.cpp"
	"Rainbow3D.cpp"
)

target_include_directories(Rainbow3D PUBLIC ${PROJECT_SOURCE_DIR}/Include)