#include "Render/Graphics/GraphicsUtility.h"
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl.h>
namespace Rainbow3D {

	class DX11GraphicsUtility : public GraphicsUtility {
	public:
		DX11GraphicsUtility(ID3D11Device* device, IDXGISwapChain* swapchain) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);
		}
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	};
	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain) {
		ID3D11Device* _device = reinterpret_cast<ID3D11Device*>(device->GetNativePointer());
		IDXGISwapChain* _swapchain = reinterpret_cast<IDXGISwapChain*>(swapchain->GetNativePointer());

		return UniquePtr<GraphicsUtility>(new DX11GraphicsUtility(_device, _swapchain));
	}
}