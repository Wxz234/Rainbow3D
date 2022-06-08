#pragma once
#include "Render/Graphics/PostProcess.h"
#include "Shader/LogLuminancePS.h"
#include "Shader/ToneMappingPS.h"
namespace Rainbow3D {
	class ToneMapping : public PostProcess {
	public:
		ToneMapping(ID3D11Device* device, IDXGISwapChain* swapchain, uint32 w, uint32 h) : PostProcess(device, swapchain, w, h) {
			Create_internal_ps_context();
			Create_default_sampler();
			create_default_vs();

			m_Device->CreatePixelShader(LogLuminancePS, sizeof(LogLuminancePS), nullptr, &m_log_luminance_ps);
			m_Device->CreatePixelShader(ToneMappingPS, sizeof(ToneMappingPS), nullptr, &m_draw_ps);

			CD3D11_TEXTURE2D_DESC luminance_tex_desc(DXGI_FORMAT_R32_FLOAT, 512, 512, 1, 10, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			luminance_tex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			m_Device->CreateTexture2D(&luminance_tex_desc, nullptr, &m_log_luminance_tex);
			m_Device->CreateShaderResourceView(m_log_luminance_tex.Get(), nullptr, &m_log_luminance_tex_srv);
			m_Device->CreateRenderTargetView(m_log_luminance_tex.Get(), nullptr, &m_log_luminance_tex_rtv);
		}

		void SetParameter(ID3D11ShaderResourceView* srv) {
			m_srv = srv;
		}
		void Render(ID3D11RenderTargetView* rtv) {
			m_postprocess_ps_context->VSSetShader(m_default_vs_shader.Get(), 0, 0);
			m_postprocess_ps_context->PSSetShader(m_log_luminance_ps.Get(), 0, 0);
			m_postprocess_ps_context->OMSetRenderTargets(1, m_log_luminance_tex_rtv.GetAddressOf(), nullptr);
			CD3D11_VIEWPORT viewport(0.0f, 0.0f, 512.f, 512.f);
			m_postprocess_ps_context->RSSetViewports(1, &viewport);
			m_postprocess_ps_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_postprocess_ps_context->PSSetShaderResources(0, 1, m_srv.GetAddressOf());
			m_postprocess_ps_context->PSSetSamplers(0, 1, m_default_sampler.GetAddressOf());
			m_postprocess_ps_context->Draw(3, 0);
			m_postprocess_ps_context->GenerateMips(m_log_luminance_tex_srv.Get());
			m_postprocess_ps_context->PSSetShader(m_draw_ps.Get(), 0, 0);
			m_postprocess_ps_context->OMSetRenderTargets(1, &rtv, nullptr);
			m_postprocess_ps_context->PSSetShaderResources(1, 1, m_log_luminance_tex_srv.GetAddressOf());
			viewport.Width = (float)w;
			viewport.Height = (float)h;
			m_postprocess_ps_context->RSSetViewports(1, &viewport);
			m_postprocess_ps_context->Draw(3, 0);
			commit_ps_list();
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_log_luminance_ps;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_draw_ps;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_log_luminance_tex;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_log_luminance_tex_srv;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_log_luminance_tex_rtv;
	};

	inline UniquePtr<ToneMapping> CreateToneMapping(Device* device, SwapChain* swapchain) {
		return UniquePtr<ToneMapping>(new ToneMapping(device->GetDevice(), swapchain->GetSwapChain(), swapchain->GetWidth(), swapchain->GetHeight()));
	}
}