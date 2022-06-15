#pragma once
#include "Core/CorePreDef.h"
#include "Core/CoreTypes.h"
#include "Runtime/Component/Component.h"
namespace Rainbow3D {
	struct SubMesh {
		//uint32 mode = 4;

		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<uint32> indices;
	};

	class MeshComponent : public Component {
	public:
		MeshComponent() {}
	private:
		std::vector<SubMesh> m_sub;
	};
}