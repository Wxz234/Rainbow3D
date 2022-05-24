#pragma once
#include "RObject/RObject.h"
#include <d3d11.h>
namespace Rainbow3D {
	class GraphicsObject : public RObject {
	public:
		virtual IUnknown* GetNativePointer() const noexcept = 0;
	};
}