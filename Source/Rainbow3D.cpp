#include "Rainbow3D.h"
#include <string>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Rainbow3D;

ID3DBlob* CompileVS(const std::string &str,const std::string & entrypoint) {
    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    D3DCompile(str.c_str(), str.size(), nullptr, nullptr, nullptr, entrypoint.c_str(), "vs_5_0", 0, 0, &blob, nullptr);
    return blob.Detach();
}

ID3DBlob* CompilePS(const std::string& str, const std::string& entrypoint) {
    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    D3DCompile(str.c_str(), str.size(), nullptr, nullptr, nullptr, entrypoint.c_str(), "ps_5_0", 0, 0, &blob, nullptr);
    return blob.Detach();
}

void Draw(Device* device, SwapChain* swapchain) {

	auto context = device->GetContext();

    swapchain->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device->GetDevice(), window->GetHWND(), w, h);

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
		float4 main(VSOUT vs_out) : SV_TARGET
		{
			//g_Tex.Sample(g_Sampler, vs_in.Tex);
			return g_Tex.Sample(g_Sampler, vs_out.uv);
		}
	)";

	auto vs_blob = CompileVS(v_str, "main");
	auto ps_blob = CompilePS(p_str, "main");

	return window->Run(Draw, device.Get(), swapchain.Get());
}