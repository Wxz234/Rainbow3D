#pragma once
#include "Core/CoreTypes.h"
#include "Core/Math/Vector.h"
#include <vector>
namespace Rainbow3D {
	class Mesh {
	public:
		Mesh() {}
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(const Mesh&) = default;
		Mesh& operator=(Mesh&&) = default;

		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> uv;
		std::vector<uint16> triangles;
	};
}