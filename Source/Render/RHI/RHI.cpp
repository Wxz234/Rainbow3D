#include "Render/RHI/RHI.h"
#include "Core/Math/Vector.h"
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
		void Release() { delete this; }
		void* GetNativePointer() const noexcept {
			return m_rtv.Get();
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rt;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
	};


	class dx11GraphicsList : public  CommandList {
	public:
		void Release() { delete this; }
		void ClearRTV(RenderTarget* rt, const float ColorRGBA[4]) {
			auto dx11rt = reinterpret_cast<dx11RenderTarget*>(rt);
			m_defferContext->ClearRenderTargetView(dx11rt->m_rtv.Get(), ColorRGBA);
		}

		void ClearDSV(RenderTarget* dsv, CLEAR_FLAGS flags, float depthValue, uint32 stencilValue) {
			auto dx11dst = reinterpret_cast<dx11RenderTarget*>(dsv);
			m_defferContext->ClearDepthStencilView(dx11dst->m_dsv.Get(), static_cast<uint32>(flags), depthValue, stencilValue);
		}

		void Open() {}
		void Close() {
			m_defferContext->FinishCommandList(FALSE, &m_list);
		}

		void* GetNativePointer() const noexcept {
			return m_list.Get();
		}

		Microsoft::WRL::ComPtr<ID3D11CommandList> m_list;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_defferContext;
	};

	class dx11GraphicsDevice : public GraphicsDevice {
	public:
		void Release() { delete this; }
		dx11GraphicsDevice(WindowContext* context, uint32 width, uint32 height) {

			UINT createDeviceFlags = 0;
#ifdef _DEBUG 
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_1;
			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, m_Device.GetAddressOf(), nullptr, m_Context.GetAddressOf());

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
				struct VSIN{
					float4 pos : POSITION;
					float2 uv : UV;
				};
				struct VSOUT{
					float4 mypos : SV_POSITION;
					float2 uv : PS_UV ;
				};
				VSOUT main( VSIN vs_in )
				{
					VSOUT vs_out;
					vs_out.mypos = vs_in.pos;
					vs_out.uv = vs_in.uv;
					return vs_out;
				}
			)";
			std::string p_str = R"(
				struct VSOUT{
					float4 pos : SV_POSITION;
					float2 uv : PS_UV;
				};
				Texture2D g_Tex : register(t0);
				SamplerState g_Sampler : register(s0);
				float4 main(VSOUT vs_in) : SV_TARGET
				{
					//g_Tex.Sample(g_Sampler, vs_in.Tex);
					return g_Tex.Sample(g_Sampler, vs_in.uv);
				}
			)";

			D3DCompile(v_str.c_str(), v_str.size(), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &m_vsblob, nullptr);
			D3DCompile(p_str.c_str(), p_str.size(), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &m_psblob, nullptr);
			m_Device->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_VertexShader);
			m_Device->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_PixelShader);
			const D3D11_INPUT_ELEMENT_DESC inputLayout[2] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			m_Device->CreateInputLayout(inputLayout, 2, m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), &m_pVertexLayout);

			m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
			m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

			CD3D11_SAMPLER_DESC sampler_Desc(D3D11_DEFAULT);
			//sampler_Desc.BorderColor[0] = 1.F;
			//sampler_Desc.BorderColor[1] = 0.F;
			//sampler_Desc.BorderColor[2] = 1.F;
			//sampler_Desc.BorderColor[3] = 1.F;
			//sampler_Desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			//sampler_Desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			//sampler_Desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			m_Device->CreateSamplerState(&sampler_Desc, &m_sampler);
			m_Context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());

			float point[3][6] = {
				{ -1.f, -1.f, .1f, 1.f, 0.f, 0.f},
				{ -1.f, 3.f,  .1f, 1.f, 0.f, 2.f},
				{ 3.f, -1.f,  .1f, 1.f, 2.f, 0.f}
			};
			CD3D11_BUFFER_DESC buffer_Desc(72, D3D11_BIND_VERTEX_BUFFER);
			D3D11_SUBRESOURCE_DATA InitData = {};
			InitData.pSysMem = point;
			m_Device->CreateBuffer(&buffer_Desc, &InitData, m_VS_vertex_Buffers.GetAddressOf());

			uint32 stride = 24;
			UINT offset = 0;

			m_Context->IASetVertexBuffers(0, 1, m_VS_vertex_Buffers.GetAddressOf(), &stride, &offset);
			m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_Context->IASetInputLayout(m_pVertexLayout.Get());
			CD3D11_VIEWPORT viewport(0.f, 0.f, width, height);
			m_Context->RSSetViewports(1, &viewport);
		}

		void Present() {
			ID3D11RenderTargetView* plist[] = { rt.m_rtv.Get() };
			m_Context->OMSetRenderTargets(1, plist, nullptr);
			m_Context->Draw(3, 0);
			m_swapChain->Present(1, 0);
		}

		void ExecuteCommandList(CommandList* list) {
			auto dx11list = reinterpret_cast<dx11GraphicsList*>(list);
			m_Context->ExecuteCommandList(dx11list->m_list.Get(), TRUE);
		}

		void BindRenderTarget(RenderTarget* rendertarget) {
			auto dx11rt = reinterpret_cast<dx11RenderTarget*>(rendertarget);
			m_Context->PSSetShaderResources(0, 1, dx11rt->m_srv.GetAddressOf());
		}


		void* GetNativePointer() const noexcept {
			return m_Device.Get();
		}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_psblob;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pVertexLayout;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VS_vertex_Buffers;
		dx11RenderTarget rt;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height) {
		return new dx11GraphicsDevice(context, width, height);
	}
	void DestroyGraphicsObject(GraphicsObject* object) {
		delete object;
	}

	CommandList* CreateCommandList(GraphicsDevice* device) {
		auto dx11list = new dx11GraphicsList;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);

		dx11device->m_Device->CreateDeferredContext(0, &dx11list->m_defferContext);
		return dx11list;
	}
	DXGI_FORMAT getformat(FORMAT format) {
		if (format == FORMAT::RGBA8_UNORM) {
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		return DXGI_FORMAT_D32_FLOAT;
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
		if (desc.Format == DXGI_FORMAT_D32_FLOAT) {
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}
		else {
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		}
		desc.MiscFlags = 0;
		desc.MipLevels = 1;
		dx11device->m_Device->CreateTexture2D(&desc, nullptr, &dx11rt->m_rt);

		if (desc.Format == DXGI_FORMAT_D32_FLOAT) {
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = desc.Format;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;
			dx11device->m_Device->CreateDepthStencilView(dx11rt->m_rt.Get(), &dsvDesc, &dx11rt->m_dsv);
		}
		else {
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
		}

		return dx11rt;
	}

	Texture2D* CreateTexture2DFromFile(GraphicsDevice* device, const wchar_t* filename) {
		return nullptr;
	}

}