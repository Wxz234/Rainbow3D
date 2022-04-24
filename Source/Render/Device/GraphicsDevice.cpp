#include "Render/Device/GraphicsDevice.h"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
namespace Rainbow3D {
	class WindowContext {
	public:
		HWND hwnd = NULL;
	};


	class GraphcisDevice::impl {
	public:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rt;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
	};

	GraphcisDevice::GraphcisDevice(WindowContext* context, uint32 width, uint32 height) {
		pimpl = new impl;
		UINT createDeviceFlags = 0;
#ifdef _DEBUG 
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_1;
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, pimpl->m_Device.GetAddressOf(), nullptr, pimpl->m_DeviceContext.GetAddressOf());
		
		Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
		pimpl->m_Device.As(&dxgiDevice);
		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);
		Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

		DXGI_SWAP_CHAIN_DESC1 _desc = {};
		_desc.BufferCount = 2;
		_desc.Width = width;
		_desc.Height = height;
		_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		_desc.SampleDesc.Count = 1;
		_desc.SampleDesc.Quality = 0;
		_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		_desc.Scaling = DXGI_SCALING_STRETCH;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		dxgiFactory->CreateSwapChainForHwnd(pimpl->m_Device.Get(), context->hwnd, &_desc, &fsSwapChainDesc, nullptr, &pimpl->m_swapChain);
		dxgiFactory->MakeWindowAssociation(context->hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
		pimpl->m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pimpl->m_rt));
		pimpl->m_Device->CreateRenderTargetView(pimpl->m_rt.Get(), NULL, &pimpl->m_rtv);
	}
	GraphcisDevice::~GraphcisDevice() {
		delete pimpl;
	}

	void GraphcisDevice::Present() {
		pimpl->m_swapChain->Present(1, 0);
	}

	void GraphcisDevice::ClearRTV(const float ColorRGBA[4]) {
		pimpl->m_DeviceContext->ClearRenderTargetView(pimpl->m_rtv.Get(), ColorRGBA);
	}

	GraphcisDevice* CreateGraphcisDevice(WindowContext* context, uint32 width, uint32 height) {
		return new GraphcisDevice(context, width, height);
	}

	void DestroyGraphcisDevice(GraphcisDevice* device) {
		delete device;
	}
}