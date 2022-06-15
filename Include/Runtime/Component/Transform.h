#pragma once
#include "Core/CorePreDef.h"
#include "Runtime/Component/Component.h"
namespace Rainbow3D {
	class Transform : public Component {
	public:
		Transform() {
			up = { 0,1,0 };
			forward = { 0,0,1 };
			position = { 0,0,0 };
		}

	private:
		DirectX::XMFLOAT3 up;
		DirectX::XMFLOAT3 forward;
		DirectX::XMFLOAT3 position;
	};
}