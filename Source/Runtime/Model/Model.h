#pragma once
#include "Render/Device/GraphicsDevice.h"
namespace Rainbow3D {
	class Model {

	};

	Model* CreateModelFromFile(GraphcisDevice* device, const wchar_t* file);
	void DestroyModel(Model* model);
}
