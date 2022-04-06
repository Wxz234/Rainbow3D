#include "Device.h"
namespace Rainbow3D {
	Device* CreateDevice(Window* window, uint32_t w, uint32_t h) {
		return new Device(window->GetHWND(), w, h);
	}

	void DestroyDevice(Device* device) {
		delete device;
	}
}
