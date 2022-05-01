#pragma once
#include "Core/CoreDef.h"

namespace Rainbow3D {
	class Renderer {
	public:
		DISABLE_COPY_AND_ASSIGN(Renderer)
		Renderer() {}
		virtual ~Renderer() {}
	};

	void DestroyRenderer(Renderer* renderer);
}