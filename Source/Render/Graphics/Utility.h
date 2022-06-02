#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Render/Graphics/PreDef.h"
#include "Render/Graphics/SwapChain.h"
#include "Render/Graphics/Device.h"
namespace Rainbow3D {

	class Utility {
	public:
		Utility(ID3D11Device* device, IDXGISwapChain* swapchain);

		void DrawTexture(ID3D11ShaderResourceView* srv);
		void CreateTextureFromFile(const wchar_t* file, ID3D11Resource** texture, ID3D11ShaderResourceView** srv);

		HRESULT CreateDeferredContext(ID3D11DeviceContext **defferedContext) {
			return m_Device->CreateDeferredContext(0, defferedContext);
		}

		void ExecuteContext(ID3D11CommandList* context) {
			m_Context->ExecuteCommandList(context, TRUE);
		}

		void InitBaseColorContext(ID3D11DeviceContext* baseColorContext);

		uint32 GetMaximumGpuResourceNumber() const {
			return 128;
		}
	private:

		void cacc() {

		}
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

		uint32 w;
		uint32 h;
	};

	inline UniquePtr<Utility> CreateUtility(ID3D11Device* device, IDXGISwapChain* swapchain) {
		return UniquePtr<Utility>(new Utility(device, swapchain));
	}
}