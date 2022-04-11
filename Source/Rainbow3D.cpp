#include "Rainbow3D.h"
#include <Windows.h>

void Draw(Rainbow3D::Device* device,float x) {
    float color[4] = { x,1,0,1 };
    device->ClearRTV(color);
    device->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32_t width = 800, height = 600;
    auto window = Rainbow3D::CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = Rainbow3D::CreateDevice(window, width, height);
    window->Run(Draw, device, 1.f);
    Rainbow3D::DestroyDevice(device);
    Rainbow3D::DestroyRenderWindow(window);
    return 0;
}