#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/Device.h"
#include "Render/Graphics/SwapChain.h"
#include "Shader/PostProcessVS.h"

namespace Rainbow3D {

	class PostProcess {
	public:
		PostProcess(ID3D11Device* device, IDXGISwapChain* swapchain) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
		}

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
			Microsoft::WRL::ComPtr<ID3D11CommandList> _list;
			m_postprocess_ps_context->FinishCommandList(FALSE, &_list);
			m_Context->ExecuteCommandList(_list.Get(), TRUE);
		}

		void create_default_vs() {
			m_Device->CreateVertexShader(PostProcessVS, sizeof(PostProcessVS), nullptr, &m_default_vs_shader);
		}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_cs_context;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_postprocess_ps_context;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_default_sampler;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_default_vs_shader;
	};
}