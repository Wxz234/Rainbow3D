#include "Rainbow3D.h"

using namespace Rainbow3D;

void Draw(SwapChain* swapchain, Utility* utility, ID3D11ShaderResourceView* srv) {

    utility->DrawTexture(srv);
    swapchain->Present();
}

Vector3 ACESFilm(Vector3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 978, h = 506;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device->GetDevice(), window->GetHWND(), w, h);
    auto utility = CreateUtility(device->GetDevice(), swapchain->GetSwapChain());
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> baseColorContext;
    utility->CreateDeferredContext(&baseColorContext);

    utility->InitBaseColorContext(baseColorContext.Get());

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    utility->CreateTextureFromFile(L"C:\\Users\\WangYuzhi\\Desktop\\x.bmp", nullptr, &srv);

    return window->Run(Draw, swapchain.Get(), utility.Get(), srv.Get());
}