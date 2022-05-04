#pragma once
#include "Core/CoreTypes.h"
#include "Platform/Window/Window.h"

namespace Rainbow3D {

	enum class CLEAR_FLAGS {
		DEPTH = 0x1,
		STENCIL = 0x2
	};

	enum class FORMAT {
		RGBA8_UNORM,
		D32_FLOAT
	};

	class GraphicsObject : public RainbowObject {
	public:
		virtual void* GetNativePointer() const noexcept = 0;
	};

	class GraphicsRenderTarget : public GraphicsObject {
	public:
	};

	class GraphicsList : public GraphicsObject {
	public:
		virtual void ClearRTV(GraphicsRenderTarget* rt, const float ColorRGBA[4]) = 0;
		virtual void ClearDSV(GraphicsRenderTarget* dsv, CLEAR_FLAGS flags, float depthValue, uint32 stencilValue) = 0;
		virtual void Open() = 0;
		virtual void Close() = 0;
	};

	class GraphicsDevice : public GraphicsObject {
	public:
		virtual void BindRenderTarget(GraphicsRenderTarget* rt) = 0;
		virtual void Present() = 0;
		virtual void ExecuteCommandList(GraphicsList* list) = 0;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	GraphicsList* CreateGraphicsList(GraphicsDevice* device);
	GraphicsRenderTarget* CreateRenderTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format);
}