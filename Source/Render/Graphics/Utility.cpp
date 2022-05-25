#include "Render/Graphics/Utility.h"
#include "Shader/Utility_VS.h"
#include "Shader/Utility_PS.h"
namespace Rainbow3D {
	void Utility::Init() {
		m_Device->CreateVertexShader(g_vs_r, sizeof(g_vs_r), nullptr, &m_VertexShader);
		m_Device->CreatePixelShader(g_ps_r, sizeof(g_ps_r), nullptr, &m_PixelShader);

		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_texture));
		m_Device->CreateRenderTargetView(m_texture.Get(), NULL, &m_rtv);

		const D3D11_INPUT_ELEMENT_DESC inputLayout[2] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		m_Device->CreateInputLayout(inputLayout, 2, g_vs_r, sizeof(g_vs_r), &m_pVertexLayout);

		CD3D11_SAMPLER_DESC sampler_Desc(D3D11_DEFAULT);
		m_Device->CreateSamplerState(&sampler_Desc, &m_sampler);
	
		float point[3][6] = {
			{ -1.f, -1.f, .1f, 1.f, 0.f, 1.f},
			{ -1.f,  3.f, .1f, 1.f, 0.f,-1.f},
			{  3.f, -1.f, .1f, 1.f, 2.f, 1.f}
		};
		CD3D11_BUFFER_DESC buffer_Desc(72, D3D11_BIND_VERTEX_BUFFER);
		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = point;
		m_Device->CreateBuffer(&buffer_Desc, &InitData, m_VS_vertex_Buffers.GetAddressOf());

		DXGI_SWAP_CHAIN_DESC swapchain_desc{};
		m_swapChain->GetDesc(&swapchain_desc);
	}

	void Utility::DrawTexture(ID3D11ShaderResourceView* srv) {
		m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
		m_Context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());

		UINT stride = 24;
		UINT offset = 0;

		m_Context->IASetVertexBuffers(0, 1, m_VS_vertex_Buffers.GetAddressOf(), &stride, &offset);
		m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_Context->IASetInputLayout(m_pVertexLayout.Get());

		DXGI_SWAP_CHAIN_DESC swapchain_desc{};
		m_swapChain->GetDesc(&swapchain_desc);
		CD3D11_VIEWPORT viewport(0.f, 0.f, swapchain_desc.BufferDesc.Width, swapchain_desc.BufferDesc.Height);
		m_Context->RSSetViewports(1, &viewport);

		m_Context->PSSetShaderResources(0, 1, &srv);
		ID3D11RenderTargetView* plist[] = { m_rtv.Get() };
		m_Context->OMSetRenderTargets(1, plist, nullptr);
		m_Context->Draw(3, 0);
	}
}