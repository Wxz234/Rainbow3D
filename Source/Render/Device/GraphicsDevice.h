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

	class GraphicsList : public GraphicsObject {
	public:
		virtual void ClearRTV(RenderTarget* rt, const float ColorRGBA[4]) = 0;
		virtual void Close() = 0;
	};

	class GraphicsDevice : public GraphicsObject {
	public:
		virtual void Present() = 0;
		virtual RenderTarget* GetSwapChainRenderTarget() = 0;
		virtual void ClearRTV(RenderTarget* rt,const float ColorRGBA[4]) = 0;
		virtual void ExecuteCommandList(GraphicsList* list) = 0;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	GraphicsList* CreateGraphicsList(GraphicsDevice* device);
}