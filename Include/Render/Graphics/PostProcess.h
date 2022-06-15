#pragma once
#include "Core/CorePreDef.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
#include "Render/Graphics/UberPostProcess.h"
namespace Rainbow3D {
	class PostProcess {
	public:
		PostProcess(ID3D11Device* device, IDXGISwapChain* swapchain, ID3D11RenderTargetView* rtv, uint32 w, uint32 h) {
			m_Device = device;
			m_Device->GetImmediateContext(&m_Context);
			m_Swapchain = swapchain;
			m_RTV = rtv;
			this->w = w;
			this->h = h;
			//create DIRECT_Context
			m_Device->CreateDeferredContext(0, &m_DIRECT_Context);

			//compile vs
			auto vs_str = GetUberPostProcessVS();
			Microsoft::WRL::ComPtr<ID3DBlob> defalut_vs_blob;
			D3DCompile(vs_str.c_str(), vs_str.size(), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &defalut_vs_blob, nullptr);
			m_Device->CreateVertexShader(defalut_vs_blob->GetBufferPointer(), defalut_vs_blob->GetBufferSize(), 0, &m_default_vertex_shader);
			//compile ps
			auto ps_str = GetUberPostProcessPS();
			Microsoft::WRL::ComPtr<ID3DBlob> log_luminance_ps_blob;
			D3D_SHADER_MACRO log_luminance_macro[] = { "RAINBOW_LOG_LUMINANCE_SHADER","1",nullptr,nullptr };
			D3DCompile(ps_str.c_str(), ps_str.size(), nullptr, log_luminance_macro, nullptr, "main", "ps_5_0", 0, 0, &log_luminance_ps_blob, nullptr);
			m_Device->CreatePixelShader(log_luminance_ps_blob->GetBufferPointer(), log_luminance_ps_blob->GetBufferSize(), nullptr, &m_log_luminance_ps);

			Microsoft::WRL::ComPtr<ID3DBlob> aces_blob;
			D3D_SHADER_MACRO aces_macro[] = { "RAINBOW_ACES_SHADER","1",nullptr,nullptr };
			D3DCompile(ps_str.c_str(), ps_str.size(), nullptr, aces_macro, nullptr, "main", "ps_5_0", 0, 0, &aces_blob, nullptr);
			m_Device->CreatePixelShader(aces_blob->GetBufferPointer(), aces_blob->GetBufferSize(), nullptr, &m_aces_ps);

			//create default sampler
			CD3D11_SAMPLER_DESC m_default_sampler_desc(D3D11_DEFAULT);
			m_Device->CreateSamplerState(&m_default_sampler_desc, &m_default_sampler);
			//create tonemapping
			CD3D11_TEXTURE2D_DESC log_luminance_desc(DXGI_FORMAT_R16_FLOAT, 512, 512, 1, 10, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			log_luminance_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			m_Device->CreateTexture2D(&log_luminance_desc, nullptr, &m_log_luminance_tex);
			m_Device->CreateRenderTargetView(m_log_luminance_tex.Get(), nullptr, &m_log_luminance_rtv);
			m_Device->CreateShaderResourceView(m_log_luminance_tex.Get(), nullptr, &m_log_luminance_srv);
		}

		void Render(ID3D11ShaderResourceView* srv) {
			m_DIRECT_Context->VSSetShader(m_default_vertex_shader.Get(), nullptr, 0);
			m_DIRECT_Context->PSSetShader(m_log_luminance_ps.Get(), nullptr, 0);
			m_DIRECT_Context->OMSetRenderTargets(1, m_log_luminance_rtv.GetAddressOf(), nullptr);
			CD3D11_VIEWPORT viewport(0.0f, 0.0f, 512.f, 512.f);
			m_DIRECT_Context->RSSetViewports(1, &viewport);
			m_DIRECT_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_DIRECT_Context->PSSetShaderResources(0, 1, &srv);
			m_DIRECT_Context->PSSetSamplers(0, 1, m_default_sampler.GetAddressOf());

			m_DIRECT_Context->Draw(3, 0);
			m_DIRECT_Context->GenerateMips(m_log_luminance_srv.Get());
			m_DIRECT_Context->PSSetShader(m_aces_ps.Get(), 0, 0);
			m_DIRECT_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), nullptr);
			m_DIRECT_Context->PSSetShaderResources(1, 1, m_log_luminance_srv.GetAddressOf());
			setscreenviewport(m_DIRECT_Context.Get());
			m_DIRECT_Context->Draw(3, 0);

			execute(m_DIRECT_Context.Get());
		}
	private:
		void setscreenviewport(ID3D11DeviceContext* context) {
			CD3D11_VIEWPORT viewport(0.0f, 0.0f, float(w), float(h));
			context->RSSetViewports(1, &viewport);
		}

		void execute(ID3D11DeviceContext *context) {
			Microsoft::WRL::ComPtr<ID3D11CommandList> _list;
			context->FinishCommandList(FALSE, &_list);
			m_Context->ExecuteCommandList(_list.Get(), TRUE);
		}
		
		uint32 w;
		uint32 h;
		// main device
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		// main context
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		// main Swapchain
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_Swapchain;
		// swapchain rtv
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
		// other cmdlist
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_COMPUTE_Context;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DIRECT_Context;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_COPY_Context;
		// shader
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_default_vertex_shader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_log_luminance_ps;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_aces_ps;
		//default sampler
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_default_sampler;
		// tonemapping
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_log_luminance_tex;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_log_luminance_rtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_log_luminance_srv;
	};

	inline UniquePtr<PostProcess> CreatePostProcess(Device* device, SwapChain* swapchain) {
		return UniquePtr<PostProcess>(new PostProcess(device->GetDevice(), swapchain->GetSwapChain(), swapchain->GetRTV(), swapchain->GetWidth(), swapchain->GetHeight()));
	}
}