#pragma once

#include "Runtime/Component/MeshComponent.h"
#include "Runtime/GameObject/GameObject.h"
#include <memory>
namespace Rainbow3D {
	class ModelObject : public GameObject {
	public:
		//void AddSubMesh(const SubMesh& submesh) {
		//	mesh.AddSubMesh(submesh);
		//}
	private:
		MeshComponent mesh;
	};

	std::unique_ptr<ModelObject> CreateModelObject(const wchar_t*file);
}