#include "Core/Log/Log.h"
#include "Runtime/Model/Model.h"
#include "Utility/JSON/json.hpp"
#include <string>
namespace Rainbow3D {
	Model* _creategltffromfile(GraphcisDevice* device, const wchar_t* file) {
		return nullptr;
	}


	Model* CreateModelFromFile(GraphcisDevice* device, const wchar_t* file) {
		std::wstring filename = file;
		if (filename.ends_with(L".gltf")) {
			return _creategltffromfile(device, file);
		}
		else {
			Rainbow3D_Error("This format is not supported.");
		}
		return nullptr;
	}

	void DestroyModel(Model* model) {
		delete model;
	}
}