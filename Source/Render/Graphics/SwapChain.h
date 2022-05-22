#pragma once
#include "Core/CoreTypes.h"
#include "RObject/RObject.h"
#include "Platform/Window/Window.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/Device.h"
namespace Rainbow3D {
	class SwapChain : public RObject {
	public:
		virtual void Present() = 0;
	};

	UniquePtr<SwapChain> CreateSwapChain(Device* device, RWindow* window, uint32 w, uint32 h);
}