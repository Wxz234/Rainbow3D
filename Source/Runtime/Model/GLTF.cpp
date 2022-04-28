#include "Runtime/Model/GLTF.h"

namespace Rainbow3D {
	//Model* _creategltffromfile(GraphcisDevice* device, const wchar_t* file) {
	//	std::ifstream i(file);
	//	if (!i.is_open()) {
	//		Rainbow3D_Error("Can't open the file.");
	//		return nullptr;
	//	}

	//	nlohmann::json j = nlohmann::json::parse(i, nullptr, false);
	//	if (j.is_discarded()) {
	//		Rainbow3D_Error("Can't pares JSON.");
	//		return nullptr;
	//	}
	//	auto version = j["asset"]["version"];
	//	if (!version.is_string() || version != "2.0") {
	//		Rainbow3D_Error("This version is not supported.");
	//		return nullptr;
	//	}

	//	return nullptr;
	//}
}