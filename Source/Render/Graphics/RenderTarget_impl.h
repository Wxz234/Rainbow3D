#pragma once 
#include "Render/Graphics/RenderTarget.h"
#include <wrl.h>
namespace Rainbow3D {
	class dx11RenderTarget : public RenderTarget {
	public:
		dx11RenderTarget(ID3D11Texture2D* texture2D, ID3D11ShaderResourceView* srv = nullptr, ID3D11RenderTargetView* rtv = nullptr, ID3D11UnorderedAccessView* uav = nullptr) {
			m_texture = texture2D;
			m_srv = srv;
			m_rtv = rtv;
			m_uav = uav;
		}
		virtual ID3D11ShaderResourceView* GetSRV() {
			return m_srv.Get();
		}
		virtual ID3D11RenderTargetView* GetRTV() {
			return m_rtv.Get();
		}
		virtual ID3D11UnorderedAccessView* GetUAV() {
			return m_uav.Get();
		}
		IUnknown* GetNativePointer() const noexcept {
			return m_texture.Get();
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;
	};
}