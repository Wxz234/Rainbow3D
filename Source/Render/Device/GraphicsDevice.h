#pragma once
#include "Core/CoreTypes.h"
#include "Core/CoreDef.h"
#include "Platform/Window/Window.h"
namespace Rainbow3D {
	class GraphcisDevice final {
	public:
		DISABLE_COPY_AND_ASSIGN(GraphcisDevice)

		GraphcisDevice(WindowContext* context, uint32 width, uint32 height);
		~GraphcisDevice();

		void Present();
		void ClearRTV(const float ColorRGBA[4]);
	private:
		class impl;
		impl* pimpl;
	};

	GraphcisDevice* CreateGraphcisDevice(WindowContext* context, uint32 width, uint32 height);
	void DestroyGraphcisDevice(GraphcisDevice* device);
}