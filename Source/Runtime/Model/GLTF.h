#pragma once
#include "Runtime/Model/Model.h"
namespace Rainbow3D {
	class GLTFModel : public Model {
	public:
	};

	Model* CreateGLTFModelFromFile(const wchar_t* file);
}
