#include "Rainbow3D.h"
#include "ThirdParty/DirectXTex/WICTextureLoader/WICTextureLoader11.h"
using namespace Rainbow3D;

void Draw(RWindow *window,SwapChain* swapchain, PostProcess* postprocess, ID3D11ShaderResourceView* srv, Utility* util) {
    


    swapchain->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 1000, h = 800;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.Get(), window.Get(), w, h);

    ID3D11Resource* tex = nullptr;
    ID3D11ShaderResourceView* srv = nullptr;
    CoInitialize(nullptr);
    DirectX::CreateWICTextureFromFile(device->GetDevice(), L"C:\\Users\\WangYuzhi\\Desktop\\no.png", &tex, &srv);
    auto util = CreateUtility(device->GetDevice(), swapchain->GetSwapChain());

    window->Show();
    return 1;
}