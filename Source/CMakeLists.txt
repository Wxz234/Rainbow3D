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