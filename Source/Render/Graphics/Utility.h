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

		void Init() {

		}
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	};

	inline UniquePtr<Utility> CreateUtility(ID3D11Device* device, IDXGISwapChain* swapchain) {
		return UniquePtr<Utility>(new Utility(device, swapchain));
	}
}