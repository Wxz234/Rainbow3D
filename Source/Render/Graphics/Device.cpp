#include "Render/Graphics/Device.h"
#include "Render/Graphics/RenderTarget_impl.h"
#include <d3d11_4.h>
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

		UniquePtr<RenderTarget> CreateRenderTarget(FORMAT format, uint32 w, uint32 h, uint32 bind_flag) {
			Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
			Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav;

			CD3D11_TEXTURE2D_DESC desc((DXGI_FORMAT)format, w, h, 1, 0, bind_flag);
			m_Device->CreateTexture2D(&desc, nullptr, &tex);

			if (bind_flag & BIND_SHADER_RESOURCE) {
				CD3D11_SHADER_RESOURCE_VIEW_DESC srv_desc(tex.Get(), D3D11_SRV_DIMENSION_TEXTURE2D, (DXGI_FORMAT)format);
				m_Device->CreateShaderResourceView(tex.Get(), &srv_desc, &srv);
			}
			if (bind_flag & BIND_RENDER_TARGET) {
				CD3D11_RENDER_TARGET_VIEW_DESC rtv_desc(tex.Get(), D3D11_RTV_DIMENSION_TEXTURE2D, (DXGI_FORMAT)format);
				m_Device->CreateRenderTargetView(tex.Get(), &rtv_desc, &rtv);
			}
			if (bind_flag & BIND_UNORDERED_ACCESS) {
				CD3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc(tex.Get(), D3D11_UAV_DIMENSION_TEXTURE2D, (DXGI_FORMAT)format);
				m_Device->CreateUnorderedAccessView(tex.Get(), &uav_desc, &uav);
			}

			return UniquePtr<RenderTarget>(new dx11RenderTarget(tex.Get(), srv.Get(), rtv.Get(), uav.Get()));
		}

		IUnknown* GetNativePointer() const noexcept {

			return m_Device.Get();
		}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
	};

	UniquePtr<Device> CreateDevice() {
		return UniquePtr<Device>(new DX11Device);
	}
}