#include "Rainbow3D.h"
using namespace Rainbow3D;

void Draw(SwapChain *swapchain) {
    swapchain->Present(1);
}
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"RainbowEditor", w, h, false);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.Get(), window.Get(), w, h);
    auto scene = CreateScene();
    auto box = CreateModel("Box", L"C:\\Users\\WangYuzhi\\Desktop\\glTF-Sample-Models-master\\2.0\\Sponza\\glTF\\Sponza.gltf");
    window->Show();
    return window->RunLoop(Draw, swapchain.Get());
}