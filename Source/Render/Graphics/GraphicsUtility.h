#pragma once
#include "RObject/RObject.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
namespace Rainbow3D {
	class GraphicsUtility : public RObject {
		
	};

	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain);
}