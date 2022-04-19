#pragma once
#include "Runtime/Object/Object.h"
namespace Rainbow3D {
	class Model : public RObject {
	public:
	};

	Model* CreateModelFromFile(const wchar_t* file);
}