#include "Render/Graphics/GraphicsUtility.h"
#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl.h>
namespace Rainbow3D {

	class DX11GraphicsUtility : public GraphicsUtility {
	public:
		DX11GraphicsUtility() {

		}


		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	};
	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain) {
		return nullptr;
	}
}