#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/GraphicsObject.h"
#include "Render/Graphics/RenderTarget.h"
#include "Render/Graphics/Common.h"
namespace Rainbow3D {
	class Device : public GraphicsObject {
	public:
		virtual UniquePtr<RenderTarget> CreateRenderTarget(FORMAT format, uint32 w, uint32 h, uint32 bind_flag = BIND_SHADER_RESOURCE) = 0;
	};

	UniquePtr<Device> CreateDevice();
}