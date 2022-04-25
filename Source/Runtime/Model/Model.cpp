#include "Core/Log/Log.h"
#include "Runtime/Model/Model.h"
#include <string>
namespace Rainbow3D {
	Model* CreateModelFromFile(GraphcisDevice* device, const wchar_t* file) {
		std::wstring filename = file;
		if (filename.ends_with(L".gltf")) {

		}
		else {
			Rainbow3D_Error("This format is not supported.");
		}
		return nullptr;
	}
}