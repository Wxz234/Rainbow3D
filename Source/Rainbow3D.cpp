#include "Rainbow3D.h"

using namespace Rainbow3D;

void Draw(SwapChain *swapchain,Device *device) {


    
    swapchain->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto  device = CreateDevice();
    auto swapchain = CreateSwapChain(device->GetDevice(), window->GetHWND(), w, h);
    window->Show();
    window->Run(Draw, swapchain.Get(), device.Get());
    return 0;
}