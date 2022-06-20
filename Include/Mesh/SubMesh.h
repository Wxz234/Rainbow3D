#pragma once
#include "Core/TypeDefine.h"
#include <DirectXMath.h>
#include <vector>
namespace Rainbow3D {
	struct SubMesh {
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<uint16> indices;
	};
}
