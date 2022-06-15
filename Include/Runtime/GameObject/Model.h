#pragma once
#include "Core/CorePreDef.h"
#include "Core/Pointer/UniquePtr.h"
#include "Runtime/Component/Transform.h"
#include "Runtime/Component/MaterialComponent.h"
#include "Runtime/Component/MeshComponent.h"
#include "Runtime/GameObject/GameObject.h"

namespace Rainbow3D {
	class Model : public GameObject {
	public:
		Model(const std::string& name = "DefaultModel") : GameObject(name) {}
	private:
		MeshComponent m_mesh;
	};

	UniquePtr<Model> CreateModel(const std::string& name, const wchar_t* file);
}