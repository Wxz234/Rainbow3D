#pragma once
#include "Core/CoreTypes.h"
#include "Core/CoreDef.h"
#include "Platform/Window/Window.h"

namespace Rainbow3D {

	class GraphicsList {
	public:
		GraphicsList();
		~GraphicsList();
		GraphicsList(const GraphicsList& r);
		GraphicsList(GraphicsList&& r) noexcept;
		GraphicsList& operator=(const GraphicsList& r);
		GraphicsList& operator=(GraphicsList&& r) noexcept;

		class Impl;
		Impl* GetImpl() {
			return pimpl;
		}

		void Close();
	private:
		Impl* pimpl;
	};

	class GraphicsDevice final {
	public:
		DISABLE_COPY_AND_ASSIGN(GraphicsDevice)

		GraphicsDevice(WindowContext* context, uint32 width, uint32 height);
		~GraphicsDevice();

		void Present();
		void ClearRTV(const float ColorRGBA[4]);
		void ExecuteCommandList(GraphicsList* list);

		class Impl;
		Impl* GetImpl() {
			return pimpl;
		}
	private:
		Impl* pimpl;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	void DestroyGraphicsDevice(GraphicsDevice* device);

	GraphicsList* CreateGraphicsList(GraphicsDevice* device);
	void DestroyGraphicsList(GraphicsList* list);
}