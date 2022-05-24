#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/GraphicsObject.h"

namespace Rainbow3D {
	class Device : public GraphicsObject {
	public:

	};

	UniquePtr<Device> CreateDevice();
}