#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#include "Runtime/GameObject/ModelObject.h"
#include "tiny_gltf.h"
#include <filesystem>
namespace Rainbow3D {
	std::unique_ptr<ModelObject> CreateModelObject(const wchar_t* file) {
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		std::filesystem::path my_path{ file };
		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, my_path.string());

		return nullptr;
	}
}