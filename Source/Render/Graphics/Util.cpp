#include "Core/Log/Log.h"
#include "Render/Graphics/Util.h"
#include "Shader/Utility_VS.h"
#include "Shader/Utility_PS.h"

#include "ThirdParty/DirectXTex/WICTextureLoader/WICTextureLoader11.h"
#include "ThirdParty/DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
#include <string>
namespace Rainbow3D {
	Utility::Utility(ID3D11Device* device, IDXGISwapChain* swapchain) {
		m_Device = device;
		m_swapChain = swapchain;
		m_Device->GetImmediateContext(&m_Context);
		m_Device->CreateVertexShader(Utility_VS, sizeof(Utility_VS), nullptr, &m_VertexShader);
		m_Device->CreatePixelShader(Utility_PS, sizeof(Utility_PS), nullptr, &m_PixelShader);

		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_texture));
		m_Device->CreateRenderTargetView(m_texture.Get(), NULL, &m_rtv);


		DXGI_SWAP_CHAIN_DESC swapchain_desc{};
		m_swapChain->GetDesc(&swapchain_desc);
		w = swapchain_desc.BufferDesc.Width;
		h = swapchain_desc.BufferDesc.Height;

		m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
		//m_Context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());



		//m_Context->IASetVertexBuffers(0, 1, m_VS_vertex_Buffers.GetAddressOf(), &stride, &offset);
		m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//m_Context->IASetInputLayout(m_pVertexLayout.Get());

		CD3D11_VIEWPORT viewport(0.f, 0.f, w, h);
		m_Context->RSSetViewports(1, &viewport);
		//add compute shader
	}

	bool _isDDS(const std::wstring& file_str) {
		if (file_str.ends_with(L".dds") ||
			file_str.ends_with(L".ddS") ||
			file_str.ends_with(L".dDs") ||
			file_str.ends_with(L".Dds") ||
			file_str.ends_with(L".DDs") ||
			file_str.ends_with(L".DdS") ||
			file_str.ends_with(L".dDS") ||
			file_str.ends_with(L".DDS")
			) {
			return true;
		}
		return false;
	}

	void Utility::DrawTexture(ID3D11ShaderResourceView* srv) {

		ID3D11RenderTargetView* drawRTV[] = { m_rtv.Get() };
		m_Context->OMSetRenderTargets(1, drawRTV, nullptr);

		m_Context->Draw(3, 0);
	}

	void Utility::CreateTextureFromFile(const wchar_t* file, ID3D11Resource** texture, ID3D11ShaderResourceView** srv) {

		auto hr = CoInitialize(NULL);
		if (!(hr == S_OK || hr == S_FALSE)) {
		
			return;
		}
		if (_isDDS(file)) {
			DirectX::CreateDDSTextureFromFile(m_Device.Get(), file, texture, srv);
		}
		else {
			DirectX::CreateWICTextureFromFile(m_Device.Get(), file, texture, srv);
		}
	}

	void Utility::InitBaseColorContext(ID3D11DeviceContext* baseColorContext) {

	}
}