#include "Core/Log/Log.h"
#include "Render/Graphics/GraphicsUtility.h"
#include "Render/Graphics/RenderTarget_impl.h"
#include "ThirdParty/DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
#include "ThirdParty/DirectXTex/WICTextureLoader/WICTextureLoader11.h"
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "ThirdParty/imgui/imgui_impl_dx11.h"
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <objbase.h>
#include <winerror.h>
#include <string>
namespace Rainbow3D {

	bool _isDDS(const std::wstring& file_str) {
		if (file_str.ends_with(L".dds") ||
			file_str.ends_with(L".ddS") ||
			file_str.ends_with(L".dDs") ||
			file_str.ends_with(L".Dds") ||
			file_str.ends_with(L".DDs") ||
			file_str.ends_with(L".DdS") ||
			file_str.ends_with(L".dDS") ||
			file_str.ends_with(L".DDS")
			) {
			return true;
		}
		return false;
	}

	class DX11GraphicsUtility : public GraphicsUtility {
	public:
		DX11GraphicsUtility(ID3D11Device* device, IDXGISwapChain* swapchain) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);

			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_rt));
			m_Device->CreateRenderTargetView(m_rt.Get(), NULL, &m_rtv);

			std::string v_str = R"(
				struct Vs_In{
					float4 pos : POSITION;
					float2 uv : TEXCOORD;
				};
				struct Vs_Out{
					float4 mypos : SV_POSITION;
					float2 uv : TEXCOORD ;
				};
				Vs_Out main( Vs_In vs_in )
				{
					Vs_Out vs_out;
					vs_out.mypos = vs_in.pos;
					vs_out.uv = vs_in.uv;
					return vs_out;
				}
			)";

			std::string p_str = R"(
				struct Vs_Out{
					float4 pos : SV_POSITION;
					float2 uv : TEXCOORD;
				};
				Texture2D g_Tex : register(t0);
				SamplerState g_Sampler : register(s0);
				float4 main(Vs_Out vs_out) : SV_TARGET
				{
					return g_Tex.Sample(g_Sampler, vs_out.uv);
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
			m_Device->CreateSamplerState(&sampler_Desc, &m_sampler);
			m_Context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());

			float point[3][6] = {
				{ -1.f, -1.f, 0.1f, 1.f, 0.f, 1.f},
				{ -1.f,  3.f, 0.1f, 1.f, 0.f,-1.f},
				{  3.f, -1.f, 0.1f, 1.f, 2.f, 1.f}
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

			DXGI_SWAP_CHAIN_DESC desc{};
			m_swapChain->GetDesc(&desc);
			CD3D11_VIEWPORT viewport(0.f, 0.f, (float)desc.BufferDesc.Width, (float)desc.BufferDesc.Height);
			m_Context->RSSetViewports(1, &viewport);
			imgui_isinit = false;
		}
		~DX11GraphicsUtility() {
			if (imgui_isinit) {
				ImGui_ImplDX11_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();
			}
		}

		UniquePtr<RenderTarget> CreateRenderTargetFromFile(const wchar_t* file) {
			Microsoft::WRL::ComPtr<ID3D11Resource> tex;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
			auto hr = CoInitialize(NULL);
			if (!(hr == S_OK || hr == S_FALSE)) {
				RAINBOW_LOG(LogVerbosity::Error, "Failed to call CoInitialize.");
				return nullptr;
			}
			if (_isDDS(file)) {
				DirectX::CreateDDSTextureFromFile(m_Device.Get(), file, &tex, &srv);
			}
			else {
				DirectX::CreateWICTextureFromFile(m_Device.Get(), file, &tex, &srv);
			}
			return UniquePtr<RenderTarget>(new dx11RenderTarget(tex.Get(), srv.Get()));
		}

		void PresentRenderTarget(RenderTarget* rendertarget) {
			auto srv = rendertarget->GetSRV();
			m_Context->PSSetShaderResources(0, 1, &srv);
			ID3D11RenderTargetView* plist[] = { m_rtv.Get() };
			m_Context->OMSetRenderTargets(1, plist, nullptr);
			m_Context->Draw(3, 0);
			if (imgui_isinit) {
				//ImGui::Render();

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				bool show_demo_window = true;
				ImGui::ShowDemoWindow(&show_demo_window);
				ImGui::Begin("Another Window", &show_demo_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				static int counter = 0;
				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::Text("counter = %d", counter);
				ImGui::End();
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
			m_swapChain->Present(1, 0);
		}

		void InitImGui(HWND hwnd) {
			if (!imgui_isinit) {
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				ImGui_ImplWin32_Init(hwnd);
				ImGui_ImplDX11_Init(m_Device.Get(), m_Context.Get());
				imgui_isinit = true;
			}
		}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rt;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;

		Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_psblob;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pVertexLayout;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VS_vertex_Buffers;
		bool imgui_isinit;
	};
	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain) {
		ID3D11Device* _device = reinterpret_cast<ID3D11Device*>(device->GetNativePointer());
		IDXGISwapChain* _swapchain = reinterpret_cast<IDXGISwapChain*>(swapchain->GetNativePointer());
		return UniquePtr<GraphicsUtility>(new DX11GraphicsUtility(_device, _swapchain));
	}
}