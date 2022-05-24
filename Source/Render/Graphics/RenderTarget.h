#pragma once 
#include "Render/Graphics/GraphicsObject.h"
#include <d3d11.h>
namespace Rainbow3D {
	class RenderTarget : public GraphicsObject {
	public:
		virtual ID3D11ShaderResourceView* GetSRV() = 0;
		virtual ID3D11RenderTargetView* GetRTV() = 0;
		virtual ID3D11UnorderedAccessView* GetUAV() = 0;
	};
}