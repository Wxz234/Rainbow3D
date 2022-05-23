#include "Render/Graphics/GraphicsUtility.h"
#include "ThirdParty/DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
#include "ThirdParty/DirectXTex/WICTextureLoader/WICTextureLoader11.h"
#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
namespace Rainbow3D {

	class DX11GraphicsUtility : public GraphicsUtility {
	public:
		DX11GraphicsUtility(ID3D11Device *device,IDXGISwapChain *swapchain) {
			m_Device = device;
			m_swapChain = swapchain;
			m_Device->GetImmediateContext(&m_Context);

			//D3DCompile

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
		}

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

		//UniquePtr<Texture2D> CreateTextureFromFile(const wchar_t* file) {
		//	std::wstring file_str = file;
		//	if (_isDDS(file_str)) {
		//		//DirectX::CreateDDSTextureFromFile(m_Device.Get(), file,)
		//	}
		//	return nullptr;
		//}

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

		Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_psblob;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pVertexLayout;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VS_vertex_Buffers;
	};
	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain) {
		ID3D11Device* _device = reinterpret_cast<ID3D11Device*>(device->GetNativeObject());
		IDXGISwapChain* _swapchain = reinterpret_cast<IDXGISwapChain*>(swapchain->GetNativeObject());
		return UniquePtr<GraphicsUtility>(new DX11GraphicsUtility(_device, _swapchain));
	}
}