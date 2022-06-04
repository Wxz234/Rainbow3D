#pragma once
#include "Core/CoreTypes.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"

#include <cstring>

namespace Rainbow3D {

	enum class PostProcessEffect {
		ToneMapping
	};

	class PostProcess {
	public:
		PostProcess(ID3D11Device* device, IDXGISwapChain* swapchain, PostProcessEffect effect) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
			m_effect = effect;

			m_Device->CreateDeferredContext(0, &m_postprocess_context);
		}

		void Process(ID3D11RenderTargetView *rtv,ID3D11ShaderResourceView *srv) {

		}
	//		m_Device = device;
	//		m_swapChain = swapchain;
	//		m_Device->GetImmediateContext(&m_Context);
	//		m_flag = flag;

	//		DXGI_SWAP_CHAIN_DESC swapchain_desc{};
	//		m_swapChain->GetDesc(&swapchain_desc);
	//		w = swapchain_desc.BufferDesc.Width;
	//		h = swapchain_desc.BufferDesc.Height;

	//		m_Device->CreateDeferredContext(0, &m_postprocess_context);

	//		CD3D11_BUFFER_DESC wh_desc(16, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	//		m_Device->CreateBuffer(&wh_desc, nullptr, &m_copy_buffer);
	//		m_Device->CreateComputeShader(CopyTexture, sizeof(CopyTexture), nullptr, &m_copy_shader);
	//		CD3D11_SAMPLER_DESC m_copy_sampler_Desc(D3D11_DEFAULT);
	//		m_Device->CreateSamplerState(&m_copy_sampler_Desc, &m_copy_sample);
	//		CD3D11_TEXTURE2D_DESC m_output_tex_Desc(DXGI_FORMAT_R16G16B16A16_FLOAT, w, h, 1, 1, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	//		m_Device->CreateTexture2D(&m_output_tex_Desc, nullptr, &m_output_tex);
	//		m_Device->CreateUnorderedAccessView(m_output_tex.Get(), nullptr, &m_output_uav);
	//		m_Device->CreateShaderResourceView(m_output_tex.Get(), nullptr, &m_output_srv);

	//		if (m_flag & RAINBOW_TONEMAPPING) {

	//			m_Device->CreateDeferredContext(0, &m_tonemapping_context);
	//			CD3D11_TEXTURE2D_DESC m_lum_tex_Desc(DXGI_FORMAT_R16_FLOAT, w, h, 1, 1, D3D11_BIND_UNORDERED_ACCESS);
	//			m_Device->CreateTexture2D(&m_lum_tex_Desc, nullptr, &m_lum_tex);
	//			m_Device->CreateUnorderedAccessView(m_lum_tex.Get(), nullptr, &m_lum_uav);
	//			m_Device->CreateComputeShader(ToneMapping, sizeof(ToneMapping), nullptr, &m_tonemapping_shader);
	//		}
	//	}

	//	void Render(ID3D11ShaderResourceView* srv, ID3D11DepthStencilView* dsv) {

	//		uint32 texture_message[4] = { w,h,0,0 };
	//		D3D11_MAPPED_SUBRESOURCE mappedRes{};
	//		m_postprocess_context->Map(m_copy_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	//		std::memcpy(mappedRes.pData, texture_message, 16);
	//		m_postprocess_context->Unmap(m_copy_buffer.Get(), 0);

	//		m_postprocess_context->CSSetShader(m_copy_shader.Get(), 0, 0);
	//		m_postprocess_context->CSSetConstantBuffers(0, 1, m_copy_buffer.GetAddressOf());
	//		m_postprocess_context->CSSetShaderResources(0, 1, &srv);
	//		m_postprocess_context->CSSetUnorderedAccessViews(0, 1, m_output_uav.GetAddressOf(), nullptr);
	//		m_postprocess_context->CSSetSamplers(0, 1, m_copy_sample.GetAddressOf());
	//		m_postprocess_context->Dispatch(w / 8 + 1, h / 8 + 1, 1);

	//		if (m_flag & RAINBOW_TONEMAPPING) {
	//			m_tonemapping_context->CSSetShader(m_tonemapping_shader.Get(), 0, 0);
	//			m_tonemapping_context->CSSetShaderResources(0, 1, m_output_srv.GetAddressOf());
	//			m_tonemapping_context->CSSetUnorderedAccessViews(0, 1, m_lum_uav.GetAddressOf(), nullptr);
	//			m_tonemapping_context->CSSetConstantBuffers(0, 1, m_copy_buffer.GetAddressOf());
	//			m_tonemapping_context->Dispatch(w / 8 + 1, h / 8 + 1, 1);
	//		}

	//		Microsoft::WRL::ComPtr<ID3D11CommandList> post_cmd_list;
	//		m_postprocess_context->FinishCommandList(TRUE, &post_cmd_list);
	//		m_Context->ExecuteCommandList(post_cmd_list.Get(), TRUE);

	//		if (m_flag & RAINBOW_TONEMAPPING) {
	//			Microsoft::WRL::ComPtr<ID3D11CommandList> post_tonemapping_list;
	//			m_tonemapping_context->FinishCommandList(TRUE, &post_tonemapping_list);
	//			m_Context->ExecuteCommandList(post_tonemapping_list.Get(), TRUE);
	//		}

	//	}

	//	ID3D11ShaderResourceView* GetOutput() {
	//		return m_output_srv.Get();
	//	}
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

		PostProcessEffect m_effect;
	//	//
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_context;
	//	//
	//	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_copy_shader;
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> m_copy_buffer;
	//	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_copy_sample;
	//	//
	//	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_output_tex;
	//	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_output_uav;
	//	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_output_srv;
	//	//tonemapping
	//	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_lum_tex;
	//	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_lum_uav;
	//	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_tonemapping_context;
	//	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_tonemapping_shader;
	//	uint32 m_flag;

		uint32 w;
		uint32 h;
	};

	inline UniquePtr<PostProcess> CreatePostProcess(Device* device, SwapChain* swapchain, PostProcessEffect effect) {
		return UniquePtr<PostProcess>(new PostProcess(device->GetDevice(), swapchain->GetSwapChain(), effect));
	}
}