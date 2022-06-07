#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
#include "Shader/PostProcessVS.h"
#include "Shader/KEyeHistogramClear.h"
#include "Shader/HistogramExposure.h"
#include "Shader/AutoExposure.h"
#include "Shader/ACES.h"
#include <cstring>
namespace Rainbow3D {

	class PostProcess {
	public:
		PostProcess(ID3D11Device* device, IDXGISwapChain* swapchain,uint32 w,uint32 h) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
			m_Device->CreateDeferredContext(0, &m_postprocess_context);
			m_Device->CreateDeferredContext(0, &m_postprocess_ps_context);
			this->w = w;
			this->h = h;

			CD3D11_SAMPLER_DESC m_default_sampler_Desc(D3D11_DEFAULT);
			m_Device->CreateSamplerState(&m_default_sampler_Desc, &m_default_sampler);
		}

		virtual void Render(ID3D11RenderTargetView* rtv) = 0;
	protected:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_context;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_ps_context;

		Microsoft::WRL::ComPtr <ID3D11SamplerState> m_default_sampler;
		uint32 w;
		uint32 h;
	};
	class ToneMapping : public PostProcess {

	public:

		ToneMapping(ID3D11Device* device, IDXGISwapChain* swapchain, uint32 w, uint32 h) : PostProcess(device, swapchain, w, h) {
			CD3D11_BUFFER_DESC exposureHistogramupload_desc(16, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
			m_Device->CreateBuffer(&exposureHistogramupload_desc, nullptr, &m_exposureHistogram_upload);
			m_Device->CreateComputeShader(KEyeHistogramClear, sizeof(KEyeHistogramClear), nullptr, &m_clearHistogram_shader);
			input_w = 0;
			input_h = 0;

			CD3D11_BUFFER_DESC exposureHistogram_desc(512, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, 4);
			m_Device->CreateBuffer(&exposureHistogram_desc, nullptr, &m_exposureHistogram);
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = 0;		
			uavDesc.Buffer.Flags = 0;
			uavDesc.Buffer.NumElements = 128;	
			m_Device->CreateUnorderedAccessView(m_exposureHistogram.Get(), &uavDesc, &m_exposureHistogram_uav);
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.FirstElement = 0;			// 起始元素的索引
			srvDesc.Buffer.NumElements = 128;
			m_Device->CreateShaderResourceView(m_exposureHistogram.Get(), &srvDesc, &m_exposureHistogram_srv);
			m_Device->CreateComputeShader(HistogramExposure, sizeof(HistogramExposure), nullptr, &m_createHistogram_shader);
			m_Device->CreateComputeShader(AutoExposure, sizeof(AutoExposure), nullptr, &m_autoexposure_shader);

			CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_R32_FLOAT, 1, 1, 1, 1, D3D11_BIND_UNORDERED_ACCESS);

			m_Device->CreateTexture2D(&texDesc, nullptr, &m_output_tex);
			m_Device->CreateUnorderedAccessView(m_output_tex.Get(), nullptr, &m_autoexposure_uav);
			m_Device->CreateVertexShader(PostProcessVS, sizeof(PostProcessVS), nullptr, &m_default_vs_shader);
			m_Device->CreatePixelShader(ACES, sizeof(ACES), nullptr, &m_aces_shader);
		}

		void SetParameter(ID3D11ShaderResourceView *srv) {
			Microsoft::WRL::ComPtr<ID3D11Resource> _res;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> _tex;
			srv->GetResource(&_res);
			_res.As(&_tex);
			D3D11_TEXTURE2D_DESC desc{};
			_tex->GetDesc(&desc);
			input_w = desc.Width;
			input_h = desc.Height;
			m_srv = srv;
		}
		void Render(ID3D11RenderTargetView* rtv) {

			float para[4] = { 1.0f / 18.0f,0.5f,(float)input_w,(float)input_h };
			D3D11_MAPPED_SUBRESOURCE mappedRes{};
			m_postprocess_context->Map(m_exposureHistogram_upload.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
			std::memcpy(mappedRes.pData, para, 16);
			m_postprocess_context->Unmap(m_exposureHistogram_upload.Get(), 0);
			m_postprocess_context->CSSetShader(m_clearHistogram_shader.Get(), nullptr, 0);
			m_postprocess_context->CSSetUnorderedAccessViews(0, 1, m_exposureHistogram_uav.GetAddressOf(), nullptr);

			m_postprocess_context->Dispatch(8, 1, 1);
			m_postprocess_context->CSSetShader(m_createHistogram_shader.Get(), nullptr, 0);
			m_postprocess_context->CSSetConstantBuffers(0, 1, m_exposureHistogram_upload.GetAddressOf());
			m_postprocess_context->CSSetSamplers(0, 1, m_default_sampler.GetAddressOf());
			m_postprocess_context->CSSetShaderResources(0, 1, m_srv.GetAddressOf());
			uint32 Dispatchx = 0, Dispatchy = 0;
			if (input_w % 32 == 0) {
				Dispatchx = input_w / 32;
			}
			else {
				Dispatchx = input_w / 32 + 1; 
			}
			if (input_h % 32 == 0) {
				Dispatchy = input_h / 32;
			}
			else {
				Dispatchy = input_h / 32 + 1;
			}
			m_postprocess_context->Dispatch(Dispatchx, Dispatchy, 1);
			m_postprocess_context->CSSetShader(m_autoexposure_shader.Get(), 0, 0);
			m_postprocess_context->CSSetUnorderedAccessViews(0, 1, m_autoexposure_uav.GetAddressOf(), nullptr);
			m_postprocess_context->CSSetShaderResources(0, 1, m_exposureHistogram_srv.GetAddressOf());
			m_postprocess_context->Dispatch(1, 1, 1);
			Microsoft::WRL::ComPtr<ID3D11CommandList> post_cs_list;
			m_postprocess_context->FinishCommandList(FALSE, &post_cs_list);
			m_Context->ExecuteCommandList(post_cs_list.Get(), TRUE);

			m_postprocess_ps_context->VSSetShader(m_default_vs_shader.Get(), 0, 0);
			m_postprocess_ps_context->PSSetShader(m_aces_shader.Get(), 0, 0);
			m_postprocess_ps_context->PSSetShaderResources(0, 1, m_srv.GetAddressOf());
			m_postprocess_ps_context->PSSetSamplers(0, 1, m_default_sampler.GetAddressOf());
			CD3D11_VIEWPORT viewport(0.f, 0.f, w, h);
			m_postprocess_ps_context->RSSetViewports(1, &viewport);
			m_postprocess_ps_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			ID3D11RenderTargetView* ppRenderTargetViews[1] = { rtv };
			m_postprocess_ps_context->OMSetRenderTargetsAndUnorderedAccessViews(1, ppRenderTargetViews, nullptr, 1, 1, m_autoexposure_uav.GetAddressOf(), nullptr);
			m_postprocess_ps_context->Draw(3, 0);
			Microsoft::WRL::ComPtr<ID3D11CommandList> post_ps_list;
			m_postprocess_ps_context->FinishCommandList(FALSE, &post_ps_list);
			m_Context->ExecuteCommandList(post_ps_list.Get(), TRUE);
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_exposureHistogram_upload;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_exposureHistogram;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_output_tex;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_exposureHistogram_uav;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_exposureHistogram_srv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_autoexposure_uav;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_clearHistogram_shader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_createHistogram_shader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_autoexposure_shader;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_default_vs_shader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_aces_shader;
		uint32 input_w;
		uint32 input_h;
	};

	inline UniquePtr<ToneMapping> CreateToneMapping(Device* device, SwapChain* swapchain) {
		return UniquePtr<ToneMapping>(new ToneMapping(device->GetDevice(), swapchain->GetSwapChain(), swapchain->GetWidth(), swapchain->GetHeight()));
	}
}