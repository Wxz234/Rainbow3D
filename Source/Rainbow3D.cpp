#include <Windows.h>
#include <functional>
#include "Rainbow3D.h"
#include "Decive.h"
#include "Window.h"

void Draw(Device& device,float x) {
    float color[4] = { x,1,0,1 };
    device.ClearRTV(color);
    device.Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32_t width = 800, height = 600;
    Window window(L"Rainbow3D", width, height);
    Device device(window.GetHWND(), width, height);
    window.Run(Draw, std::ref(device), 1.f);
    return 0;
}