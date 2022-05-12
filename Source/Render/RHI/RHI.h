#pragma once
#include "Core/CoreTypes.h"
#include "Platform/Window/Window.h"

namespace Rainbow3D {

	enum class CLEAR_FLAGS {
		DEPTH = 0x1,
		STENCIL = 0x2
	};

	enum class FORMAT {
		UNKNOWN,
		RGBA8_UNORM,
		D32_FLOAT
	};

	class Texture2D : public RainbowObject {
	public:
		virtual FORMAT GetFormat() const noexcept = 0;
	};

	class RenderTarget : public RainbowObject {
	public:
		virtual Texture2D* GetTexture2D() = 0;
	};

	class DepthStencilTarget : public RainbowObject {
	public:
		virtual Texture2D* GetTexture2D() = 0;
	};

	class CommandList : public RainbowObject {
	public:
		virtual void ClearRTV(RenderTarget* rt, const float ColorRGBA[4]) = 0;
		virtual void ClearDSV(DepthStencilTarget* dst, CLEAR_FLAGS flags, float depthValue, uint32 stencilValue) = 0;
		virtual void Open() = 0;
		virtual void Close() = 0;
	};

	class GraphicsDevice : public RainbowObject {
	public:
		virtual void Present(RenderTarget* rt) = 0;
		virtual void Present(Texture2D* texture) = 0;
		virtual void ExecuteCommandList(CommandList* list) = 0;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	CommandList* CreateCommandList(GraphicsDevice* device);
	RenderTarget* CreateRenderTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format);
	DepthStencilTarget* CreateDepthStencilTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format);
	Texture2D* CreateTexture2DFromFile(GraphicsDevice* device, const wchar_t* filename);
}