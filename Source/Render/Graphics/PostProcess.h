#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
#include "Shader/HistogramExposure.h"
#include "Shader/Exposure.h"
#include "Shader/ToneMappingPS.h"
#include "Shader/PostProcessVS.h"
#include <cstring>
namespace Rainbow3D {

	class PostProcess {
	public:
		PostProcess(ID3D11Device* device, IDXGISwapChain* swapchain,uint32 w,uint32 h) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
			m_Device->CreateDeferredContext(0, &m_postprocess_context);
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

		Microsoft::WRL::ComPtr <ID3D11SamplerState> m_default_sampler;
		uint32 w;
		uint32 h;
	};
	class ToneMapping : public PostProcess {

	public:

		ToneMapping(ID3D11Device* device, IDXGISwapChain* swapchain, uint32 w, uint32 h) : PostProcess(device, swapchain, w, h) {

		}

		void SetParameter(ID3D11Resource *input, ID3D11ShaderResourceView *srv,float deltatime) {

		}
		void Render(ID3D11RenderTargetView* rtv) {

		}
	private:


		float m_deltatime;
		uint32 input_w;
		uint32 input_h;
		bool firstframe;
	};

	inline UniquePtr<ToneMapping> CreateToneMapping(Device* device, SwapChain* swapchain) {
		return UniquePtr<ToneMapping>(new ToneMapping(device->GetDevice(), swapchain->GetSwapChain(), swapchain->GetWidth(), swapchain->GetHeight()));
	}
}