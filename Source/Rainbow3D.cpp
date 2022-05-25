#include "Rainbow3D.h"

using namespace Rainbow3D;

void Draw() {
   
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    Device device;
    SwapChain swapchain(device.m_Device.Get(), window->GetHWND(), w, h);
    {

    }

    window->Show();
    window->Run(Draw);
    return 0;
}