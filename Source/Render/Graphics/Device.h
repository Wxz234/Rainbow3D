#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "RObject/RObject.h"
#include "Render/Graphics/NativeObject.h"
namespace Rainbow3D {
	class Device : public NativeObject {
	public:

	};

	UniquePtr<Device> CreateDevice();
}