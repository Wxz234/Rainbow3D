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


	class dx11RenderTarget : public RenderTarget {
	public:

		void* GetNativePointer() const noexcept {
			return m_rtv.Get();
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rt;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	};


	class dx11GraphicsList : public  GraphicsList {
	public:

		void ClearRTV(RenderTarget* rt, const float ColorRGBA[4]) {
			auto dx11rt = reinterpret_cast<dx11RenderTarget*>(rt);
			m_defferContext->ClearRenderTargetView(dx11rt->m_rtv.Get(), ColorRGBA);
		}
		void Close() {
			m_defferContext->FinishCommandList(FALSE, &m_list);
		}

		void* GetNativePointer() const noexcept {
			return m_list.Get();
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
			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&rt.m_rt));
			m_Device->CreateRenderTargetView(rt.m_rt.Get(), NULL, &rt.m_rtv);

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

		virtual void ClearRTV(RenderTarget* mrt,const float ColorRGBA[4]) {
			auto dx11rt = reinterpret_cast<dx11RenderTarget*>(mrt);
			m_DeviceContext->ClearRenderTargetView(dx11rt->m_rtv.Get(), ColorRGBA);
		}
		virtual void ExecuteCommandList(GraphicsList* list) {
			auto dx11list = reinterpret_cast<dx11GraphicsList*>(list);
			m_DeviceContext->ExecuteCommandList(dx11list->m_list.Get(), TRUE);
		}

		virtual RenderTarget* GetSwapChainRenderTarget() {
			return &rt;
		}

		void* GetNativePointer() const noexcept {
			return m_Device.Get();
		}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_psblob;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pVertexLayout;

		dx11RenderTarget rt;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height) {
		return new dx11GraphicsDevice(context, width, height);
	}
	void DestroyGraphicsObject(GraphicsObject* object) {
		delete object;
	}

	GraphicsList* CreateGraphicsList(GraphicsDevice* device) {
		auto dx11list = new dx11GraphicsList;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);

		dx11device->m_Device->CreateDeferredContext(0, &dx11list->m_defferContext);
		return dx11list;
	}
	DXGI_FORMAT getformat(FORMAT format) {
		//if (format == FORMAT::RGBA8_UNORM) {
		//	return DXGI_FORMAT_R8G8B8A8_UNORM;
		//}
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	RenderTarget* CreateRenderTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format) {
		auto dx11rt = new dx11RenderTarget;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.Format = getformat(format);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = 0;
		desc.MipLevels = 1;
		dx11device->m_Device->CreateTexture2D(&desc, nullptr, &dx11rt->m_rt);
		//dx11device->m_Device->CreateTexture2D()

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = desc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		dx11device->m_Device->CreateRenderTargetView(dx11rt->m_rt.Get(), &renderTargetViewDesc, &dx11rt->m_rtv);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		dx11device->m_Device->CreateShaderResourceView(dx11rt->m_rt.Get(), &srvDesc, &dx11rt->m_srv);

		return dx11rt;
	}

}