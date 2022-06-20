#pragma once
#include "Mesh/SubMesh.h"
#include "Runtime/Component/Component.h"
#include <vector>
namespace Rainbow3D {

	class MeshComponent : public Component {
	public:
		void AddSubMesh(const SubMesh &submesh) {
			submeshes.push_back(submesh);
		}
	private:
		std::vector<SubMesh> submeshes;
	};
}