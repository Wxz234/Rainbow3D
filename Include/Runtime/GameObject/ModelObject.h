#pragma once
#include "CorePreDef.h"
#include "Runtime/GameObject/GameObject.h"
namespace Rainbow3D {
	class ModelObject : public GameObject {
	public:
		void LoadMesh() {}
	};

	std::unique_ptr<ModelObject> CreateModelObject(const wchar_t*file);
}