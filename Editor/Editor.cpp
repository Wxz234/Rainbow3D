#include "Rainbow3D.h"

using namespace Rainbow3D;
void Draw(Device* device, SwapChain* swapchain) {
    swapchain->Present(1);
    device->WaitSwapChain(swapchain->GetSwapChain());
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"RainbowEditor", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.get(), window.get(), w, h);
    window->Show();
    return window->RunLoop(Draw, device.get(), swapchain.get());
}