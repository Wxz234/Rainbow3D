#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/PreDef.h"

namespace Rainbow3D {
	class Device {
	public:
		Device() {
			UINT createDeviceFlags = 0;
#ifdef _DEBUG 
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_1;
			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, m_Device.GetAddressOf(), nullptr, m_Context.GetAddressOf());
		}
		ID3D11Device* GetDevice() const {
			return m_Device.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
	};

	inline UniquePtr<Device> CreateDevice() {
		return UniquePtr<Device>(new Device);
	}
}