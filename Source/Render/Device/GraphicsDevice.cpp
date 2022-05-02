#include "Render/Device/GraphicsDevice.h"
#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <string>
namespace Rainbow3D {

	class WindowContext {
	public:
		HWND hwnd = NULL;
	};

	class dx11GraphicsList : public  GraphicsList {
	public:
		void Close() {
			m_defferContext->FinishCommandList(FALSE, &m_list);
		}
		Microsoft::WRL::ComPtr<ID3D11CommandList> m_list;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_defferContext;
	};

	class dx11GraphicsDevice :public GraphicsDevice {
	public:
		dx11GraphicsDevice(WindowContext* context, uint32 width, uint32 height) {

			UINT createDeviceFlags = 0;
#ifdef _DEBUG 
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_1;
			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf());

			Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
			m_Device.As(&dxgiDevice);
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

			dxgiFactory->CreateSwapChainForHwnd(m_Device.Get(), context->hwnd, &_desc, &fsSwapChainDesc, nullptr, &m_swapChain);
			dxgiFactory->MakeWindowAssociation(context->hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_rt));
			m_Device->CreateRenderTargetView(m_rt.Get(), NULL, &m_rtv);

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

			D3DCompile(v_str.c_str(), v_str.size(), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &m_vsblob, nullptr);
			D3DCompile(p_str.c_str(), p_str.size(), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &m_psblob, nullptr);
			m_Device->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_VertexShader);
			m_Device->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_PixelShader);
			const D3D11_INPUT_ELEMENT_DESC inputLayout = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			m_Device->CreateInputLayout(&inputLayout, 1, m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), &m_pVertexLayout);

			m_DeviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
			m_DeviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);
		}

		void GraphicsDevice::Present() {
			m_swapChain->Present(1, 0);
		}

		virtual void ClearRTV(const float ColorRGBA[4]) {
			m_DeviceContext->ClearRenderTargetView(m_rtv.Get(), ColorRGBA);
		}
		virtual void ExecuteCommandList(GraphicsList* list) {
			auto dx11list = reinterpret_cast<dx11GraphicsList*>(list);
			m_DeviceContext->ExecuteCommandList(dx11list->m_list.Get(), TRUE);
		}

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

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height) {
		return new dx11GraphicsDevice(context, width, height);
	}
	void DestroyGraphicsDevice(GraphicsDevice* device) {
		delete device;
	}

	GraphicsList* CreateGraphicsList(GraphicsDevice* device) {
		auto dx11list = new dx11GraphicsList;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);

		dx11device->m_Device->CreateDeferredContext(0, &dx11list->m_defferContext);
		return dx11list;
	}
	void DestroyGraphicsList(GraphicsList* list) {
		delete list;
	}

}