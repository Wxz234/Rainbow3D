#include <Windows.h>
#include "Rainbow3D.h"
using namespace Rainbow3D;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphcisDevice(window->GetContext(), width, height);

    window->Run(
        [](GraphcisDevice* device) { 
            float color[4] = { 0.42f,0.27f,0.88f,1.f }; 
            device->ClearRTV(color); 
            device->Present(); 
        }, 
        device
    );
    DestroyGraphcisDevice(device);
    DestroyRenderWindow(window);
    return 0;
}