#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Runtime/GameObject/Model.h"
#include "tinygltf/tiny_gltf.h"
namespace Rainbow3D {

	//void LoadPostiom

	//void LoadMeshComponent(nlohmann::json &node, std::vector<MeshComponent>& mesh) {
	//	auto mesh_num = node["meshes"].size();
	//	mesh.resize(mesh_num);

	//	for (std::size_t i = 0;i < mesh_num; ++i) {
	//		auto attributes_size = node["meshes"][i]["primitives"].size();
	//		for (std::size_t j = 0;j < attributes_size; ++j) {
	//			
	//		}
	//	}
	//}
	using namespace tinygltf;
	UniquePtr<Model> CreateModel(const std::string& name, const wchar_t* file) {
		//tinygltf::Model model;
		//TinyGLTF loader;
		//std::string err;
		//std::string warn;
		//bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, file);
		//LoadMeshComponent(j, mesh);
		return nullptr;
	}
}