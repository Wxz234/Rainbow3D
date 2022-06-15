#pragma once
#include "Core/CorePreDef.h"
#include "Core/Pointer/UniquePtr.h"
#include "Runtime/GameObject/Camera.h"
namespace Rainbow3D {

	class Scene {
	public:
		Scene() {}
		void SetMainCamera(Camera* camera) {
			m_main_camera = camera;
		}

	private:
		Camera* m_main_camera = nullptr;
		Camera m_default_camera;
	};

	inline UniquePtr<Scene> CreateScene() {
		return MakeUnique<Scene>();
	}
}