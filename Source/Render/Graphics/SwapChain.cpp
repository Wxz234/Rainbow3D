#include "Render/Graphics/SwapChain.h"
#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl.h>
namespace Rainbow3D {

	class DX11SwapChain : public SwapChain {
	public:
		DX11SwapChain(ID3D11Device* device, HWND hwnd, uint32 w, uint32 h) {

			Microsoft::WRL::ComPtr<ID3D11Device> m_Device = device;
			Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
			m_Device.As(&dxgiDevice);
			Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
			dxgiDevice->GetAdapter(&dxgiAdapter);
			Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
			dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

			DXGI_SWAP_CHAIN_DESC1 _desc = {};
			_desc.BufferCount = 2;
			_desc.Width = w;
			_desc.Height = h;
			_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			_desc.SampleDesc.Count = 1;
			_desc.SampleDesc.Quality = 0;
			_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			_desc.Scaling = DXGI_SCALING_STRETCH;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
			fsSwapChainDesc.Windowed = TRUE;

			dxgiFactory->CreateSwapChainForHwnd(m_Device.Get(), hwnd, &_desc, &fsSwapChainDesc, nullptr, &m_swapChain);
			dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
		}

		void Present() {
			m_swapChain->Present(1, 0);
		}

		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	};

	UniquePtr<SwapChain> CreateSwapChain(Device* device, RWindow* window, uint32 w, uint32 h) {
		auto device_ptr = reinterpret_cast<ID3D11Device*>(device->GetNativeDevice());
		auto _context = window->GetContext();
		HWND hwnd = reinterpret_cast<HWND> (_context.context);
		return UniquePtr<SwapChain>(new DX11SwapChain(device_ptr, hwnd, w, h));
	}
}