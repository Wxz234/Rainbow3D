#pragma once
#include "RObject/RObject.h"
#include <cstddef>
namespace Rainbow3D {
	class Blob : public RObject {
		virtual void* GetBufferPointer() const noexcept = 0;
		virtual std::size_t GetBufferSize() = 0;
	};
}