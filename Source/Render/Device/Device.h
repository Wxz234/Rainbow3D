#pragma once
#include "Platform/Window/Window.h"
namespace Rainbow3D {
	class Device {
	public:
	};

	enum class DEIVCE_TYPE
	{
		D3D11
	};

	Device* CreateDevice(DEIVCE_TYPE type, Window* window);
	void DestroyDevice(Device* device);
}