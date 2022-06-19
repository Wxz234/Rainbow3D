#pragma once
#include "CorePreDef.h"
#include "Runtime/Component/Component.h"
namespace Rainbow3D {

	struct SubMesh {
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<DirectX::XMFLOAT3> normals;
		std::vector<DirectX::XMFLOAT3> tangents;
		std::vector<DirectX::XMFLOAT2> uv0;
		std::vector<DirectX::XMFLOAT2> uv1;
		std::vector<uint16> indices;
	};

	class MeshComponent : public Component {
	public:
		void AddSubMesh(const SubMesh &submesh) {
			submeshes.push_back(submesh);
		}
	private:
		std::vector<SubMesh> submeshes;
	};
}