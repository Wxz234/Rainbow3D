#pragma once
#include "RObject/RObject.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
#include "Render/Graphics/Blob.h"
#include "Render/Graphics/Common.h"

namespace Rainbow3D {
	class GraphicsUtility : public RObject {
	public:
		//virtual UniquePtr<Texture2D> CreateTextureFromFile(const wchar_t *file) = 0;
	};

	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain);
}