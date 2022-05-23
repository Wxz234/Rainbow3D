#include "Render/Graphics/Device.h"
#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl.h>
namespace Rainbow3D {

	class DX11Device : public Device {
	public:
		DX11Device() {
			UINT createDeviceFlags = 0;
#ifdef _DEBUG 
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_1;
			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, m_Device.GetAddressOf(), nullptr, m_Context.GetAddressOf());
		}

		void* GetNativeObject() const noexcept {
			return m_Device.Get();
		}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
	};

	UniquePtr<Device> CreateDevice() {
		return UniquePtr<Device>(new DX11Device);
	}
}