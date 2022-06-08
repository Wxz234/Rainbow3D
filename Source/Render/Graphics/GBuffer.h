#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/Device.h"
namespace Rainbow3D {
	class GBuffer {
	public:
		GBuffer(ID3D11Device* device, uint32 w, uint32 h) :m_Device(device), width(w), height(h) {
			CD3D11_TEXTURE2D_DESC basecolordesc(DXGI_FORMAT_R16G16B16A16_FLOAT, width, height, 1, 1, D3D11_BIND_SHADER_RESOURCE);
			m_Device->CreateTexture2D(&basecolordesc, nullptr, &m_BaseColor);
			m_Device->CreateShaderResourceView(m_BaseColor.Get(), nullptr, &m_BaseColor_srv);
		}
		ID3D11Texture2D* GetBaseColorBuffer() const {
			return m_BaseColor.Get();
		}
		ID3D11ShaderResourceView* GetBaseColorSRV() const {
			return m_BaseColor_srv.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_BaseColor;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_BaseColor_srv;
		uint32 width; 
		uint32 height;
	};
	inline UniquePtr<GBuffer> CreateGBuffer(Device* device, uint32 w, uint32 h) {
		return UniquePtr<GBuffer>(new GBuffer(device->GetDevice(), w, h));
	}
}