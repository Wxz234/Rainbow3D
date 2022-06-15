#pragma once
#include "Core/CorePreDef.h"
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"
#include "Platform/Window/Window.h"
#include "Render/Graphics/Device.h"

namespace Rainbow3D {
	class SwapChain {
	public:
		SwapChain(ID3D11Device* device, HWND hwnd, uint32 w, uint32 h) {
			Microsoft::WRL::ComPtr<ID3D11Device> m_Device = device;
			Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
			m_Device.As(&dxgiDevice);
			Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
			dxgiDevice->GetAdapter(&dxgiAdapter);
			Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

			DXGI_SWAP_CHAIN_DESC1 _desc = {};
			_desc.BufferCount = 1;
			_desc.Width = w;
			_desc.Height = h;
			_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			_desc.SampleDesc.Count = 1;
			_desc.SampleDesc.Quality = 0;
			_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			_desc.Scaling = DXGI_SCALING_STRETCH;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
			fsSwapChainDesc.Windowed = TRUE;

			dxgiFactory->CreateSwapChainForHwnd(m_Device.Get(), hwnd, &_desc, &fsSwapChainDesc, nullptr, &m_swapChain);
			dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
			this->w = w;
			this->h = h;

			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_texture));
			m_Device->CreateRenderTargetView(m_texture.Get(), NULL, &m_rtv);
		}

		void Present(uint32 sync) {
			m_swapChain->Present(sync, 0);
		}

		IDXGISwapChain* GetSwapChain() const {
			return m_swapChain.Get();
		}

		uint32 GetWidth() const {
			return w;
		}
		uint32 GetHeight() const {
			return h;
		}

		ID3D11RenderTargetView* GetRTV()const {
			return m_rtv.Get();
		}

		ID3D11Texture2D* GetBuffer() const {
			return m_texture.Get();
		}

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
		uint32 w; 
		uint32 h;
	};

	inline UniquePtr<SwapChain> CreateSwapChain(Device *device,RWindow *window, uint32 w, uint32 h) {
		return UniquePtr<SwapChain>(new SwapChain(device->GetDevice(), window->GetHWND(), w, h));
	}
}