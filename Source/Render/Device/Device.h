#pragma once
#include "Platform/Window/Window.h"
namespace Rainbow3D {
	class Device {
	public:
		virtual ~Device() {}
		virtual void Present() = 0;
		virtual void ClearRTV(const float ColorRGBA[4]) = 0;
	};

	enum class GRAPHICS_TYPE
	{
		D3D11
	};

	Device* CreateDevice(GRAPHICS_TYPE type, Window* window);
	void DestroyDevice(Device* device);
}