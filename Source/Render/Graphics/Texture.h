#pragma once
#include "RObject/RObject.h"
#include "Render/Graphics/NativeObject.h"
#include "Render/Graphics/View.h"
namespace Rainbow3D {
	class Texture2D : public NativeObject {
	public:
		virtual ShaderResourceView* GetSRV() = 0;
		virtual RenderTargetView* GetRTV() = 0;
	};
}