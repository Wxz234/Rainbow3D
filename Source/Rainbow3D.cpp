#include "Rainbow3D.h"
#include "ThirdParty/DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
using namespace Rainbow3D;

void Draw(SwapChain* swapchain, ToneMapping* tm, ID3D11Resource* tex, ID3D11ShaderResourceView* srv) {
    
    auto rtv = swapchain->GetRTV();
    tm->SetParameter(srv);
    tm->Render(rtv);
    //postprocess->Process(rtv, srv);
    swapchain->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 1000, h = 800;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.Get(), window.Get(), w, h);
    //auto postprocess = CreatePostProcess(device.Get(), swapchain.Get(), TONEMAPPING);
    ID3D11Resource* tex = nullptr;
    ID3D11ShaderResourceView* srv = nullptr;
    CoInitialize(nullptr);
    auto p = DirectX::CreateDDSTextureFromFile(device->GetDevice(), L"D:\\File\\Code\\Rainbow\\tonemapper\\data\\example_images\\cornell_box.dds", &tex, &srv);
    ID3D11Texture2D* mytex;
    tex->QueryInterface(&mytex);
    D3D11_TEXTURE2D_DESC pDesc{};
    mytex->GetDesc(&pDesc);

    auto tm = CreateToneMapping(device.Get(), swapchain.Get());
    window->Show();
    return RunLoop(Draw, swapchain.Get(), tm.Get(), tex, srv);
}