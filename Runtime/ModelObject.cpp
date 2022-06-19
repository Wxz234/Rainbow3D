#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#include "Runtime/GameObject/ModelObject.h"
#include "tiny_gltf.h"
#include <filesystem>
#include <string>
#include <cstddef>


namespace Rainbow3D {
	std::unique_ptr<ModelObject> CreateModelObject(const wchar_t* file) {
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		std::filesystem::path my_path{ file };
		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, my_path.string());

		std::size_t mesh_size = model.meshes.size();
		for (std::size_t mesh_index = 0; mesh_index < mesh_size; ++mesh_index) {
			std::size_t primitives_size = model.meshes[mesh_index].primitives.size();
			for (std::size_t primitives_index = 0; primitives_index < primitives_size; ++primitives_index) {
				tinygltf::Primitive& primitive = model.meshes[mesh_index].primitives[primitives_index];
				{
					const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes["POSITION"]];
					const tinygltf::BufferView& positionBufferView = model.bufferViews[positionAccessor.bufferView];
					const tinygltf::Buffer& positionBuffer = model.buffers[positionBufferView.buffer];
					const float* positions = reinterpret_cast<const float*>(&positionBuffer.data[positionBufferView.byteOffset + positionAccessor.byteOffset]);
					for (std::size_t i = 0; i < positionAccessor.count; ++i) {

						//auto str = std::to_string(positions[i * 3 + 0]) + " " + std::to_string(positions[i * 3 + 1]) + " " + std::to_string(positions[i * 3 + 2]) + "\n";
						//OutputDebugStringA(str.c_str());
					}
				}

				// index
				if (primitive.indices != -1) {
					const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
					const tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
					const tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferView.buffer];

					const uint16* indices = reinterpret_cast<const uint16*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
					for (std::size_t i = 0; i < indicesAccessor.count; ++i) {

					//	//auto str = std::to_string(positions[i * 3 + 0]) + " " + std::to_string(positions[i * 3 + 1]) + " " + std::to_string(positions[i * 3 + 2]) + "\n";
					//	//OutputDebugStringA(str.c_str());
					}
				}
				else {
					// 0 1 2 3 4 5 6 7 8
				}
				// normal

				// TANGENT

				// uv0

				// uv1

			}
		}

		return nullptr;
	}
}