#include "Core/Log/Log.h"
#include "Core/Math/Vector.h"
#include "Render/RHI/RHI.h"
#include "Render/RHI/DDSTextureLoader11.h"
#include "Render/RHI/WICTextureLoader11.h"
#include "Platform/Window/WindowContext.h"

#include <Windows.h>
#include <winerror.h>
#include <d3d11_4.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <string>
#include <mutex>

#include <boost/shared_ptr.hpp>

void p() {
	boost::shared_ptr<int> x;
}

namespace Rainbow3D {

	DXGI_FORMAT getdxgiformat(FORMAT format) {
		if (format == FORMAT::UNKNOWN) {
			return DXGI_FORMAT_UNKNOWN;
		}
		else if(format == FORMAT::RGBA8_UNORM) {
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		return DXGI_FORMAT_D32_FLOAT;
	}

	FORMAT getformat(DXGI_FORMAT format) {
		if (format == DXGI_FORMAT_R8G8B8A8_UNORM) {
			return FORMAT::RGBA8_UNORM;
		}
		else if (format == DXGI_FORMAT_D32_FLOAT) {
			return FORMAT::D32_FLOAT;
		}
		return FORMAT::UNKNOWN;
	}

	class dx11Texture2D : public Texture2D {
	public:
		void Release() { delete this; }

		FORMAT GetFormat() const noexcept {
			return m_format;
		}
		FORMAT m_format = FORMAT::UNKNOWN;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	};

	class dx11RenderTarget : public RenderTarget {
	public:
		void Release() { delete this; }
		Texture2D* GetTexture2D() { return &m_tex; }
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
		dx11Texture2D m_tex;
	};

	class dx11DepthStencilTarget : public DepthStencilTarget {
	public:
		void Release() { delete this; }
		Texture2D* GetTexture2D() { return &m_tex; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
		dx11Texture2D m_tex;
	};

	class dx11GraphicsList : public  CommandList {
	public:
		void Release() { delete this; }
		void ClearRTV(RenderTarget* rt, const float ColorRGBA[4]) {
			auto dx11rt = reinterpret_cast<dx11RenderTarget*>(rt);
			m_defferContext->ClearRenderTargetView(dx11rt->m_rtv.Get(), ColorRGBA);
		}

		void ClearDSV(DepthStencilTarget* dst, CLEAR_FLAGS flags, float depthValue, uint32 stencilValue) {
			auto dx11dst = reinterpret_cast<dx11DepthStencilTarget*>(dst);
			m_defferContext->ClearDepthStencilView(dx11dst->m_dsv.Get(), static_cast<uint32>(flags), depthValue, stencilValue);
		}

		void Open() {}
		void Close() {
			m_defferContext->FinishCommandList(FALSE, &m_list);
		}

		Microsoft::WRL::ComPtr<ID3D11CommandList> m_list;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_defferContext;
	};

	class dx11GraphicsDevice : public GraphicsDevice {
	public:
		void Release() { delete this; }
		dx11GraphicsDevice(WindowContext* context, uint32 width, uint32 height) {
			temp_present_rt = nullptr;

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
			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&rt.m_tex.m_texture));
			m_Device->CreateRenderTargetView(rt.m_tex.m_texture.Get(), NULL, &rt.m_rtv);

			//D3DCompile()
			std::string v_str = R"(
				struct VSIN{
					float4 pos : POSITION;
					float2 uv : TEXCOORD;
				};
				struct VSOUT{
					float4 mypos : SV_POSITION;
					float2 uv : TEXCOORD ;
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
					float2 uv : TEXCOORD;
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
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
				{ -1.f, -1.f, .1f, 1.f, 0.f, 1.f},
				{ -1.f, 3.f,  .1f, 1.f, 0.f,-1.f},
				{ 3.f, -1.f,  .1f, 1.f, 2.f, 1.f}
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

		void Present(RenderTarget* rendertarget) {
			auto dx11rt = reinterpret_cast<dx11RenderTarget*>(rendertarget);
			if (temp_present_rt != dx11rt) {
				temp_present_rt = dx11rt;
				m_Context->PSSetShaderResources(0, 1, dx11rt->m_tex.m_srv.GetAddressOf());
			}
			ID3D11RenderTargetView* plist[] = { rt.m_rtv.Get() };
			m_Context->OMSetRenderTargets(1, plist, nullptr);
			m_Context->Draw(3, 0);
			m_swapChain->Present(1, 0);
		}

		void Present(Texture2D* texture) {
			auto dx11texture = reinterpret_cast<dx11Texture2D*>(texture);

			m_Context->PSSetShaderResources(0, 1, dx11texture->m_srv.GetAddressOf());
			
			ID3D11RenderTargetView* plist[] = { rt.m_rtv.Get() };
			m_Context->OMSetRenderTargets(1, plist, nullptr);
			m_Context->Draw(3, 0);
			m_swapChain->Present(1, 0);
		}

		void ExecuteCommandList(CommandList* list) {
			auto dx11list = reinterpret_cast<dx11GraphicsList*>(list);
			m_Context->ExecuteCommandList(dx11list->m_list.Get(), TRUE);
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
		dx11RenderTarget* temp_present_rt;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height) {
		return new dx11GraphicsDevice(context, width, height);
	}

	CommandList* CreateCommandList(GraphicsDevice* device) {
		auto dx11list = new dx11GraphicsList;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);

		dx11device->m_Device->CreateDeferredContext(0, &dx11list->m_defferContext);
		return dx11list;
	}

	RenderTarget* CreateRenderTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format) {
		auto dx11rt = new dx11RenderTarget;
		dx11rt->m_tex.m_format = format;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.Format = getdxgiformat(format);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = 0;
		desc.MipLevels = 1;
		dx11device->m_Device->CreateTexture2D(&desc, nullptr, &dx11rt->m_tex.m_texture);
	
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = desc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		dx11device->m_Device->CreateRenderTargetView(dx11rt->m_tex.m_texture.Get(), &renderTargetViewDesc, &dx11rt->m_rtv);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		dx11device->m_Device->CreateShaderResourceView(dx11rt->m_tex.m_texture.Get(), &srvDesc, &dx11rt->m_tex.m_srv);
		
		return dx11rt;
	}

	DepthStencilTarget* CreateDepthStencilTarget(GraphicsDevice* device, uint32 width, uint32 height, FORMAT format) {
		auto dx11dst = new dx11DepthStencilTarget;
		dx11dst->m_tex.m_format = format;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.Format = getdxgiformat(format);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		desc.MiscFlags = 0;
		desc.MipLevels = 1;
		dx11device->m_Device->CreateTexture2D(&desc, nullptr, &dx11dst->m_tex.m_texture);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = desc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		dx11device->m_Device->CreateDepthStencilView(dx11dst->m_tex.m_texture.Get(), &dsvDesc, &dx11dst->m_dsv);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		dx11device->m_Device->CreateShaderResourceView(dx11dst->m_tex.m_texture.Get(), &srvDesc, &dx11dst->m_tex.m_srv);

		return dx11dst;
	}

	bool isDDS(const std::wstring &str) {
		if (str.size() < 4) {
			return false;
		}
		auto r_iter = str.rbegin();
		if (*r_iter == L's' || *r_iter == L'S') {
			++r_iter;
		}
		else{
			return false;
		}
		if (*r_iter == L'd' || *r_iter == L'D') {
			++r_iter;
		}
		else {
			return false;
		}
		if (*r_iter == L'd' || *r_iter == L'D') {
			++r_iter;
		}
		else {
			return false;
		}
		if (*r_iter == L'.') {
			return true;
		}
	
		return false;
	}

	Texture2D* CreateTexture2DFromFile(GraphicsDevice* device, const wchar_t* filename) {
		std::wstring file = filename;
		auto temp = new dx11Texture2D;
		auto dx11device = reinterpret_cast<dx11GraphicsDevice*>(device);
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		HRESULT hr;
		static HRESULT hr_once;
		static std::once_flag flag_once;
		std::call_once(flag_once, []() {  hr_once = CoInitialize(nullptr); });

		if (isDDS(file)) {
			hr = DirectX::CreateDDSTextureFromFile(dx11device->m_Device.Get(), file.c_str(), &resource, &srv);
		}
		else {
			hr = DirectX::CreateWICTextureFromFile(dx11device->m_Device.Get(), file.c_str(), &resource, &srv);
		}
		if (FAILED(hr) || FAILED(hr_once)) {
			Rainbow3D_Error("HR");
			return nullptr;
		}
		D3D11_RESOURCE_DIMENSION pResourceDimension = {};
		resource->GetType(&pResourceDimension);
		if (pResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D) {
			temp->m_srv = srv;
			resource.As(&temp->m_texture);
			D3D11_TEXTURE2D_DESC desc = {};
			temp->m_texture->GetDesc(&desc);
			temp->m_format = getformat(desc.Format);
			return temp;
		}
		return temp;
	}

}