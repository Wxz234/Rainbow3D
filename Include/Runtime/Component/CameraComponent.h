#pragma once
#include "Core/CorePreDef.h"
#include "Runtime/Component/Component.h"
namespace Rainbow3D {
	class CameraComponent : public Component {
	public:
		CameraComponent() {
			FovAngleY = 0.785398163f; // pi / 4
			AspectRatio = 1.f;
			NearZ = 0.3f;
			FarZ = 1000.f;
		}

		CameraComponent(float FovAngleY, float AspectRatio, float NearZ, float FarZ) :FovAngleY(FovAngleY), AspectRatio(AspectRatio), NearZ(NearZ), FarZ(FarZ) {}

		DirectX::XMMATRIX GetPerspective() const {
			return DirectX::XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
		}
	private:
		float FovAngleY;
		float AspectRatio;
		float NearZ;
		float FarZ;
	};
}