#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/SwapChain.h"
#include "Render/Graphics/Device.h"
namespace Rainbow3D {
	class Utility {
	public:
		Utility(ID3D11Device* device, IDXGISwapChain* swapchain) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
		}

		void Init();

		void DrawTexture(ID3D11ShaderResourceView* srv);

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pVertexLayout;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VS_vertex_Buffers;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	};

	inline UniquePtr<Utility> CreateUtility(ID3D11Device* device, IDXGISwapChain* swapchain) {
		return UniquePtr<Utility>(new Utility(device, swapchain));
	}
}