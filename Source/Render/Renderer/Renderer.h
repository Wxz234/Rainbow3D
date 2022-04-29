#pragma once
#include "Core/CoreDef.h"
#include "Render/Device/GraphicsDevice.h"
namespace Rainbow3D {
	class Renderer {
	public:
		DISABLE_COPY_AND_ASSIGN(Renderer)

		Renderer(GraphcisDevice* device) :_device(device) {}
	private:
		GraphcisDevice* _device;
	};
}