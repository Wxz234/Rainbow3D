#include "Rainbow3D.h"

using namespace Rainbow3D;
void Draw(Swapchain* swapchain) {
    swapchain->Present(1);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"RainbowEditor", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapchain(device.get(), window.get(), w, h);
    window->Show();
    return window->RunLoop(Draw, swapchain.get());
}