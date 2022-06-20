#pragma once
#include "Mesh/SubMesh.h"
#include "Runtime/Component/MeshComponent.h"
#include "Runtime/GameObject/GameObject.h"
#include <memory>
namespace Rainbow3D {
	class ModelObject : public GameObject {
	public:

	private:
		MeshComponent mesh;
	};

	std::unique_ptr<ModelObject> CreateModelObject(const wchar_t*file);
}