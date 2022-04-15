add_executable(Rainbow3D WIN32 
	"Rainbow3D.cpp"
	"Rainbow3D.h"
	"Logger.cpp"
	"Logger.h"
	"Vector.cpp"
	"Vector.h"
	"Device.cpp"
	"Device.h"
	"Window.cpp"
	"Window.h"
	"Model.cpp"
	"Model.h"
	"Json.cpp"
	"Json.h"
	"WICTextureLoader11.cpp"
	"WICTextureLoader11.h"
	"DDSTextureLoader11.cpp"
	"DDSTextureLoader11.h"
)

target_compile_definitions(Rainbow3D PUBLIC UNICODE)
target_link_libraries(Rainbow3D PRIVATE d3d11)