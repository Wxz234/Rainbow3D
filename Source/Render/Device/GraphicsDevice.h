#pragma once
#include "Core/CoreTypes.h"
#include "Core/CoreDef.h"
#include "Platform/Window/Window.h"
namespace Rainbow3D {

	class GraphicsDevice final {
	public:
		DISABLE_COPY_AND_ASSIGN(GraphicsDevice)

		GraphicsDevice(WindowContext* context, uint32 width, uint32 height);
		~GraphicsDevice();

		void Present();
		void ClearRTV(const float ColorRGBA[4]);

		//void* GetNativeDevice() const noexcept;
		//void* GetNativeDeviceContext() const noexcept;
	private:
		class impl;
		impl* pimpl;

		friend class GraphicsList;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	void DestroyGraphicsDevice(GraphicsDevice* device);

	class GraphicsList {
	public:

		GraphicsList(GraphicsDevice* device = nullptr);
		~GraphicsList();
		GraphicsList(const GraphicsList& r);
		GraphicsList(GraphicsList&& r) noexcept;
		GraphicsList& operator=(const GraphicsList& r);
		GraphicsList& operator=(GraphicsList&& r) noexcept;

		void FinishCommandList();
	private:
		class impl;
		impl* pimpl;
	};

	//GraphicsList *CreateGraphics
}