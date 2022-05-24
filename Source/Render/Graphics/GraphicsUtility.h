#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "RObject/RObject.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
#include "Render/Graphics/RenderTarget.h"
#include <Windows.h>
namespace Rainbow3D {
	class GraphicsUtility : public RObject {
	public:
		virtual UniquePtr<RenderTarget> CreateRenderTargetFromFile(const wchar_t* file) = 0;
		virtual void PresentRenderTarget(RenderTarget* rendertarget) = 0;
		virtual void InitImGui(HWND hwnd) = 0;
	};

	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain);
}