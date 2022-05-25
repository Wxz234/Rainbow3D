#pragma once
#include "Core/Math/Vector.h"
#include "Core/CoreTypes.h"
#include <vector>
namespace Rainbow3D {
	struct Mesh {
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> uv;
		std::vector<uint32> indices;
	};
}