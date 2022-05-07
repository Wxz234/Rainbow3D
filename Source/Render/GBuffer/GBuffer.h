#pragma once
#include "Render/RHI/RHI.h"
namespace Rainbow3D {
	struct GBuffer {
		RenderTarget* BaseColor;
		DepthStencilTarget* DepthStencil;
	};

	
}