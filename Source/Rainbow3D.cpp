#include "Rainbow3D.h"

using namespace Rainbow3D;

void Draw(SwapChain* swapchain, Utility* utility, ID3D11ShaderResourceView* srv) {

    utility->DrawTexture(srv);
    swapchain->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device->GetDevice(), window->GetHWND(), w, h);
    auto utility = CreateUtility(device->GetDevice(), swapchain->GetSwapChain());

    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferredContext;
    utility->CreateDeferredContext(&deferredContext);

    Microsoft::WRL::ComPtr<ID3D11Resource> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    utility->CreateTextureFromFile(L"C:\\Users\\WangYuzhi\\Desktop\\x.jpg", &texture, &srv);
    return window->Run(Draw, swapchain.Get(), utility.Get(), srv.Get());
}