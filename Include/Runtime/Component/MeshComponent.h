#pragma once
#include "Mesh/SubMesh.h"
#include "Runtime/Component/Component.h"
#include <vector>
namespace Rainbow3D {


	class MeshComponent : public Component {
	public:

	private:
		std::vector<SubMesh> submeshes;
	};
}