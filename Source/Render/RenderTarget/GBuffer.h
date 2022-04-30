#pragma once
#include "Core/CoreDef.h"
#include "Render/Device/GraphicsDevice.h"
namespace Rainbow3D {
	class GBuffer {
	public:
		DISABLE_COPY_AND_ASSIGN(GBuffer)
		GBuffer(GraphcisDevice* device) : _device(device) {}
	private:
		GraphcisDevice* _device;
	};

	GBuffer* CreateGBuffer(GraphcisDevice* device);
	void DestroyGBuffer(GBuffer* gbuffer);
}