#include <Windows.h>
#include "Rainbow3D.h"
using namespace Rainbow3D;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.Get(), window.Get(), w, h);
    auto graphicsutility = CreateGraphicsUtility(device.Get(), swapchain.Get());

    auto texture2d = device->CreateTexture2D(FORMAT::R8G8B8A8_UNORM, w, h, BIND_SHADER_RESOURCE);
    window->Show();
    while (window->IsActive()) {
        window->Dispatch();
        swapchain->Present();
    }
    texture2d.Reset();
    graphicsutility.Reset();
    swapchain.Reset();
    device.Reset();
    window.Reset();
    return 0;
}