#pragma once
#include "Core/CorePreDef.h"
#include "Core/Pointer/UniquePtr.h"
#include "Runtime/Component/Transform.h"
#include "Runtime/Component/CameraComponent.h"
#include "Runtime/GameObject/GameObject.h"

namespace Rainbow3D {
	class Camera : public GameObject {
	public:
		Camera(const std::string& name = "DefaultCamera") : GameObject(name) {}

	private:
		Transform m_transform;
		CameraComponent m_cam;
	};

	inline UniquePtr<Camera> CreateCamera(const std::string& name) {
		return MakeUnique<Camera>(name);
	}
}