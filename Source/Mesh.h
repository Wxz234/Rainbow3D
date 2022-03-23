#pragma once
#include <vector>
#include <cstdint>
#include "Vector.h"
class Mesh {
public:
	Mesh() {}
	Mesh(const Mesh&) = default;
	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(const Mesh&) = default;
	Mesh& operator=(Mesh&&) noexcept = default;
private:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> uv;
	std::vector<uint32_t> indices;
};