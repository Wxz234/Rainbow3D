#pragma once
#include "Core/CoreTypes.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
#include "Shader/PostProcessVS.h"
#include "Shader/PostProcessPS.h"
namespace Rainbow3D {

	enum PostProcessEffect {
		TONEMAPPING = 1
	};

	class PostProcess {
	public:
		PostProcess(ID3D11Device* device, IDXGISwapChain* swapchain, uint32 effect,uint32 w,uint32 h) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
			m_effect = effect;

			m_Device->CreateDeferredContext(0, &m_postprocess_context);
			this->w = w;
			this->h = h;
			if (m_effect == 0) {
				m_Device->CreateVertexShader(PostProcessVS, sizeof(PostProcessVS), nullptr, &m_default_vs);
				m_Device->CreatePixelShader(PostProcessPS, sizeof(PostProcessPS), nullptr, &m_default_ps);
				CD3D11_SAMPLER_DESC sampler_Desc(D3D11_DEFAULT);
				m_Device->CreateSamplerState(&sampler_Desc, &m_default_sampler);
			}

		}

		void Process(ID3D11RenderTargetView *rtv,ID3D11ShaderResourceView *srv) {
			if (m_effect == 0) {
				CD3D11_VIEWPORT viewport(0.f, 0.f, w, h);
				m_postprocess_context->RSSetViewports(1, &viewport);
				m_postprocess_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				m_postprocess_context->VSSetShader(m_default_vs.Get(), nullptr, 0);
				m_postprocess_context->PSSetShader(m_default_ps.Get(), nullptr, 0);
				m_postprocess_context->PSSetSamplers(0, 1, m_default_sampler.GetAddressOf());
				m_postprocess_context->PSSetShaderResources(0, 1, &srv);

				ID3D11RenderTargetView* drawRTV[] = { rtv };
				m_postprocess_context->OMSetRenderTargets(1, drawRTV, nullptr);
				m_postprocess_context->Draw(3, 0);
				Microsoft::WRL::ComPtr<ID3D11CommandList> post_cmd_list;
				m_postprocess_context->FinishCommandList(FALSE, &post_cmd_list);
				m_Context->ExecuteCommandList(post_cmd_list.Get(), TRUE);
				return;
			}
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_context;
		//
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_default_vs;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_default_ps;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_default_sampler;
		uint32 m_effect;
		uint32 w;
		uint32 h;
	};

	inline UniquePtr<PostProcess> CreatePostProcess(Device* device, SwapChain* swapchain, uint32 effect) {
		return UniquePtr<PostProcess>(new PostProcess(device->GetDevice(), swapchain->GetSwapChain(), effect, swapchain->GetWidth(), swapchain->GetHeight()));
	}
}