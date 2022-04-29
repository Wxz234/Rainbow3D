#pragma once
#include "Core/CoreDef.h"
#include "Render/Device/GraphicsDevice.h"
namespace Rainbow3D {
	class Renderer {
	public:
		DISABLE_COPY_AND_ASSIGN(Renderer)

		Renderer(GraphcisDevice* device) :_device(device) {}
		virtual ~Renderer() {}
	protected:
		GraphcisDevice* _device;
	};

	void DestroyRenderer(Renderer* renderer);
}