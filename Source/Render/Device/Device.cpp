#include "Render/Device/Device.h"
#include "Render/Device/D11Device.h"

namespace Rainbow3D {
	Device* CreateDevice(GRAPHICS_TYPE type, Window* window) {
		return new D11Device(window->GetHWND(), window->GetWidth(), window->GetHeight());
	}
	void DestroyDevice(Device* device) {
		delete device;
	}

}
