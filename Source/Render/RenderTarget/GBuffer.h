#pragma once
#include "Core/CoreDef.h"
#include "Render/Device/GraphicsDevice.h"
namespace Rainbow3D {
	class GBuffer {
	public:
		DISABLE_COPY_AND_ASSIGN(GBuffer)
		GBuffer(GraphicsDevice* device) : _device(device) {}
	private:
		GraphicsDevice* _device;
	};

	GBuffer* CreateGBuffer(GraphicsDevice* device);
	void DestroyGBuffer(GBuffer* gbuffer);
}