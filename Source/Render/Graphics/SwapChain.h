#pragma once
#include "Core/CoreTypes.h"
#include "Render/Graphics/GraphicsObject.h"
#include "Render/Graphics//Device.h"
#include <Windows.h>
namespace Rainbow3D {
	class SwapChain : public GraphicsObject {
	public:
		virtual void Present() = 0;
	};

	UniquePtr<SwapChain> CreateSwapChain(Device* device, HWND hwnd, uint32 w, uint32 h);
}