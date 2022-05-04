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

	class Texture2D : public GraphicsObject {
	public:
	};

	class RenderTarget : public GraphicsObject {
	public:
	};

	class CommandList : public GraphicsObject {
	public:
		virtual void ClearRTV(RenderTarget* rt, const float ColorRGBA[4]) = 0;
		virtual void ClearDSV(RenderTarget* dsv, CLEAR_FLAGS flags, float depthValue, uint32 stencilValue) = 0;
		virtual void Open() = 0;
		virtual void Close() = 0;
	};

	class GraphicsDevice : public GraphicsObject {
	public:
		virtual void BindRenderTarget(RenderTarget* rt) = 0;
		virtual void Present() = 0;
		virtual void ExecuteCommandList(CommandList* list) = 0;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	CommandList* CreateCommandList(GraphicsDevice* device);
	RenderTarget* CreateRenderTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format);
	Texture2D* CreateTexture2DFromFile(GraphicsDevice* device, const wchar_t* filename);
}