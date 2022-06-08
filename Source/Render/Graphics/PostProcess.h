#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"

namespace Rainbow3D {

	class PostProcess {
	public:
		PostProcess(ID3D11Device* device, IDXGISwapChain* swapchain,uint32 w,uint32 h) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
		}

		virtual void Render(ID3D11RenderTargetView* rtv) = 0;
	protected:

		void Create_internal_ps_context() {
			m_Device->CreateDeferredContext(0, &m_postprocess_ps_context);
		}
		void Create_internal_cs_context() {
			m_Device->CreateDeferredContext(0, &m_postprocess_cs_context);
		}
		void Create_default_sampler() {
			CD3D11_SAMPLER_DESC m_default_sampler_Desc(D3D11_DEFAULT);
			m_Device->CreateSamplerState(&m_default_sampler_Desc, &m_default_sampler);
		}

		void commit_ps_list() {

		}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_cs_context;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_ps_context;

		Microsoft::WRL::ComPtr <ID3D11SamplerState> m_default_sampler;
		uint32 w;
		uint32 h;
	};
	class ToneMapping : public PostProcess {

	public:

		ToneMapping(ID3D11Device* device, IDXGISwapChain* swapchain, uint32 w, uint32 h) : PostProcess(device, swapchain, w, h) {
			Create_internal_cs_context();
			Create_internal_ps_context();
			CD3D11_TEXTURE2D_DESC luminance_tex_desc(DXGI_FORMAT_R32_FLOAT, 512, 512, 1, 10, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			luminance_tex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			m_Device->CreateTexture2D(&luminance_tex_desc, nullptr, &m_log_luminance_tex);
			m_Device->CreateShaderResourceView(m_log_luminance_tex.Get(), nullptr, &m_log_luminance_tex_srv);
			m_Device->CreateRenderTargetView(m_log_luminance_tex.Get(), nullptr, &m_log_luminance_tex_rtv);
		}

		void SetParameter(ID3D11ShaderResourceView *srv) {

		}
		void Render(ID3D11RenderTargetView* rtv) {

		}
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_log_luminance_tex;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_log_luminance_tex_srv;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_log_luminance_tex_rtv;
	};

	inline UniquePtr<ToneMapping> CreateToneMapping(Device* device, SwapChain* swapchain) {
		return UniquePtr<ToneMapping>(new ToneMapping(device->GetDevice(), swapchain->GetSwapChain(), swapchain->GetWidth(), swapchain->GetHeight()));
	}
}