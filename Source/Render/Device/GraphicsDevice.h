#pragma once
#include "Core/CoreTypes.h"
#include "Platform/Window/Window.h"

namespace Rainbow3D {

	class GraphicsObject {
	public:
		virtual ~GraphicsObject() {}
		virtual void* GetNativePointer() const noexcept = 0;
	};
	void DestroyGraphicsObject(GraphicsObject* object);

	class RenderTarget : public GraphicsObject {
	public:
	};

	enum class CLEAR_FLAGS {
		DEPTH = 0x1,
		STENCIL = 0x2
	};

	class GraphicsList : public GraphicsObject {
	public:
		virtual void ClearRTV(RenderTarget* rt, const float ColorRGBA[4]) = 0;
		virtual void ClearDSV(RenderTarget* dsv, CLEAR_FLAGS flags, float depthValue, uint32 stencilValue) = 0;
		virtual void SetRenderTarget(RenderTarget* rt, RenderTarget* dst) = 0;

		virtual void Close() = 0;
	};

	class GraphicsDevice : public GraphicsObject {
	public:
		virtual void Present() = 0;
		virtual RenderTarget* GetSwapChainRenderTarget() = 0;
		virtual void ClearRTV(RenderTarget* rtv,const float ColorRGBA[4]) = 0;
		virtual void ClearDSV(RenderTarget* dsv, CLEAR_FLAGS flags, float depthValue, uint32 stencilValue) = 0;
		virtual void SetRenderTarget(RenderTarget* rt, RenderTarget* dst) = 0;
		virtual void ExecuteCommandList(GraphicsList* list) = 0;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	GraphicsList* CreateGraphicsList(GraphicsDevice* device);

	enum class FORMAT {
		RGBA8_UNORM,
		D32_FLOAT
	};

	RenderTarget* CreateRenderTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format);

}