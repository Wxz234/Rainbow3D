#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "RObject/RObject.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
namespace Rainbow3D {
	class GraphicsUtility : public RObject {
	public:
	};

	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain);
}