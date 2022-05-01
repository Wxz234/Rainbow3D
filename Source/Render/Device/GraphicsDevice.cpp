#include "Render/Device/GraphicsDevice.h"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <string>
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
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_psblob;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pVertexLayout;
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

		
		//D3DCompile()
		std::string v_str = R"(
			float4 main( float4 pos : POSITION ) : SV_POSITION
			{
				return pos;
			}
		)";
		std::string p_str = R"(
			float4 main() : SV_TARGET
			{
				return float4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		)";

		D3DCompile(v_str.c_str(), v_str.size(), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &pimpl->m_vsblob, nullptr);
		D3DCompile(p_str.c_str(), p_str.size(), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &pimpl->m_psblob, nullptr);
		pimpl->m_Device->CreateVertexShader(pimpl->m_vsblob->GetBufferPointer(), pimpl->m_vsblob->GetBufferSize(), nullptr, &pimpl->m_VertexShader);
		pimpl->m_Device->CreatePixelShader(pimpl->m_psblob->GetBufferPointer(), pimpl->m_psblob->GetBufferSize(), nullptr, &pimpl->m_PixelShader);
		const D3D11_INPUT_ELEMENT_DESC inputLayout = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		pimpl->m_Device->CreateInputLayout(&inputLayout, 1, pimpl->m_vsblob->GetBufferPointer(), pimpl->m_vsblob->GetBufferSize(), &pimpl->m_pVertexLayout);
		// 将着色器绑定到渲染管线
		pimpl->m_DeviceContext->VSSetShader(pimpl->m_VertexShader.Get(), nullptr, 0);
		pimpl->m_DeviceContext->PSSetShader(pimpl->m_PixelShader.Get(), nullptr, 0);

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
	void* GraphcisDevice::GetNativeDevice() const noexcept {
		return pimpl->m_Device.Get();
	}
	void* GraphcisDevice::GetNativeDeviceContext() const noexcept {
		return pimpl->m_DeviceContext.Get();
	}

	GraphcisDevice* CreateGraphcisDevice(WindowContext* context, uint32 width, uint32 height) {
		return new GraphcisDevice(context, width, height);
	}

	void DestroyGraphcisDevice(GraphcisDevice* device) {
		delete device;
	}
}