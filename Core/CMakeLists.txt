add_library(RainbowCore STATIC 
	"Core.cpp"
)

target_compile_definitions(RainbowCore PRIVATE UNICODE _UNICODE)
target_include_directories(RainbowCore PRIVATE "${CMAKE_SOURCE_DIR}/Include")