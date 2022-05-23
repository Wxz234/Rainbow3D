#pragma once
#include "RObject/RObject.h"
namespace Rainbow3D {
	class NativeObject : public RObject {
	public:
		virtual void* GetNativeObject() const noexcept = 0;
	};
}