#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "RObject/RObject.h"
namespace Rainbow3D {
	class Device : public RObject {
	public:
		virtual void* GetNativeDevice() const noexcept = 0;
	};

	UniquePtr<Device> CreateDevice();
}