#pragma once
#include "Core/CoreDef.h"
#include "Render/Device/GraphicsDevice.h"
namespace Rainbow3D {
	class Texture {
	public:
		DISABLE_LVALUE_COPY_AND_ASSIGN(Texture)
		Texture() {}
	};
	Texture* CreateTextureFromFile(GraphicsDevice* device, const wchar_t* file);
	void DestroyTexture(Texture* texture);
}