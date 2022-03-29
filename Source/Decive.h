#pragma once
#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <cstdint>

struct Device {
    Device(HWND hwnd, uint32_t w, uint32_t h) {
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, 2, D3D11_SDK_VERSION, m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf());
    
        // First, convert our ID3D11Device1 into an IDXGIDevice1
        Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
        m_Device.As(&dxgiDevice);

        // Second, use the IDXGIDevice1 interface to get access to the adapter
        Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
        dxgiDevice->GetAdapter(&dxgiAdapter);

        // Third, use the IDXGIAdapter interface to get access to the factory
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
		
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_rt));
		m_Device->CreateRenderTargetView(m_rt.Get(), NULL, &m_rtv);
	}
	Device(const Device&) = delete;
	Device(Device&&) noexcept = default;
	Device& operator=(const Device&) = delete;
	Device& operator=(Device&&) noexcept = default;

	void Present() {
		m_swapChain->Present(1, 0);
	}

	void ClearRTV(const float ColorRGBA[4]) {
		m_DeviceContext->ClearRenderTargetView(m_rtv.Get(), ColorRGBA);
	}

private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rt;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
};

