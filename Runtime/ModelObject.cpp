#include "Runtime/GameObject/ModelObject.h"
#include "ThirdParty/DirectXMesh/DirectXMesh.h"
#include "ThirdParty/UVatlas/UVAtlas.h"
#include "ThirdParty/tinygltf/tiny_gltf.h"
namespace Rainbow3D {

	std::unique_ptr<ModelObject> CreateModelObject(const wchar_t* file) {
		return nullptr;
	}

/*	DirectX::XMFLOAT3 GetNormal(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, const DirectX::XMFLOAT3& c) {
		auto _a = DirectX::XMLoadFloat3(&a);
	//	auto _b = DirectX::XMLoadFloat3(&b);
	//	auto _c = DirectX::XMLoadFloat3(&c);
	//	auto _ab = DirectX::XMVectorSubtract(_a, _b);
	//	auto _ac = DirectX::XMVectorSubtract(_a, _c);
	//	auto normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(_ab, _ac));
	//	DirectX::XMFLOAT3 val;
	//	DirectX::XMStoreFloat3(&val, normal);
	//	retu*///rn val;
	//}

	//std::unique_ptr<ModelObject> CreateModelObject(const wchar_t* file) {
	//	std::unique_ptr<ModelObject> my_model(new ModelObject);
	//	tinygltf::Model model;
	//	tinygltf::TinyGLTF loader;
	//	std::string err;
	//	std::string warn;
	//	std::filesystem::path my_path{ file };
	//	bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, my_path.string());

	//	std::size_t mesh_size = model.meshes.size();
	//	for (std::size_t mesh_index = 0; mesh_index < mesh_size; ++mesh_index) {
	//		std::size_t primitives_size = model.meshes[mesh_index].primitives.size();
	//		for (std::size_t primitives_index = 0; primitives_index < primitives_size; ++primitives_index) {
	//			SubMesh submesh;
	//			tinygltf::Primitive& primitive = model.meshes[mesh_index].primitives[primitives_index];
	//			{
	//				const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes["POSITION"]];
	//				const tinygltf::BufferView& positionBufferView = model.bufferViews[positionAccessor.bufferView];
	//				const tinygltf::Buffer& positionBuffer = model.buffers[positionBufferView.buffer];
	//				const float* positions = reinterpret_cast<const float*>(&positionBuffer.data[positionBufferView.byteOffset + positionAccessor.byteOffset]);
	//				for (std::size_t i = 0; i < positionAccessor.count; ++i) {
	//					submesh.vertices.push_back(DirectX::XMFLOAT3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]));
	//				}
	//			}

	//			if (primitive.indices != -1) {
	//				const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
	//				const tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
	//				const tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferView.buffer];

	//				const uint16* indices = reinterpret_cast<const uint16*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
	//				for (std::size_t i = 0; i < indicesAccessor.count; ++i) {
	//					submesh.indices.push_back(indices[i]);
	//				}
	//			}
	//			else {
	//				auto vertices_size = submesh.vertices.size();
	//				for (std::size_t i = 0; i < vertices_size; ++i) {
	//					submesh.indices.push_back(i);
	//				}
	//			}
	//			// normal

	//			if (primitive.attributes.contains("NORMAL")) {
	//				const tinygltf::Accessor& normalAccessor = model.accessors[primitive.attributes["NORMAL"]];
	//				const tinygltf::BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
	//				const tinygltf::Buffer& normalBuffer = model.buffers[normalBufferView.buffer];
	//				const float* normals = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);
	//				for (std::size_t i = 0; i < normalAccessor.count; ++i) {
	//					submesh.normals.push_back(DirectX::XMFLOAT3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]));
	//				}
	//			}
	//			else {
	//				submesh.normals.resize(submesh.vertices.size());
	//				auto triangle_size = submesh.indices.size() / 3;
	//				for (std::size_t i = 0; i < triangle_size; ++i) {
	//					auto normal = GetNormal(submesh.vertices[submesh.indices[i * 3 + 0]], submesh.vertices[submesh.indices[i * 3 + 1]], submesh.vertices[submesh.indices[i * 3 + 2]]);
	//					submesh.normals[submesh.indices[i * 3 + 0]] = normal;
	//					submesh.normals[submesh.indices[i * 3 + 1]] = normal;
	//					submesh.normals[submesh.indices[i * 3 + 2]] = normal;
	//				}
	//			}
	//			my_model->AddSubMesh(submesh);

	//		}
	//	}

	//	return my_model;
	//}
}