#pragma once
#include "Core/TypeDefine.h"
#include <DirectXMath.h>
#include <vector>
#include <cstddef>
namespace Rainbow3D {
	struct SubMesh {
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<DirectX::XMFLOAT3> normals;
		std::vector<uint16> indices;

		void ComputeNormals() {
			auto nFaces = indices.size() / 3;
			normals.resize(vertices.size());
			for (std::size_t face = 0; face < nFaces; ++face) {
				uint16 i0 = indices[face * 3];
				uint16 i1 = indices[face * 3 + 1];
				uint16 i2 = indices[face * 3 + 2];

				DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&vertices[i0]);
				DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&vertices[i1]);
				DirectX::XMVECTOR p3 = DirectX::XMLoadFloat3(&vertices[i2]);

				DirectX::XMVECTOR u = DirectX::XMVectorSubtract(p2, p1);
				DirectX::XMVECTOR v = DirectX::XMVectorSubtract(p3, p1);

				DirectX::XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(u, v));
				
				DirectX::XMStoreFloat3(&normals[i0], faceNormal);
				DirectX::XMStoreFloat3(&normals[i1], faceNormal);
				DirectX::XMStoreFloat3(&normals[i2], faceNormal);
			}
		}
	};

}
