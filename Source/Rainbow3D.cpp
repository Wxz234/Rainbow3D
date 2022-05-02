#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
using namespace Rainbow3D;

void Draw(GraphicsDevice* device, GraphicsList* list) {
    auto rt = device->GetSwapChainRenderTarget();
    float color[4] = { 0.3f,0.5f,0.0f,1.f };
    list->ClearRTV(rt, color);
    list->Close();
    device->ExecuteCommandList(list);
    color[2] = 1.0f;
    device->ClearRTV(rt, color);
    device->ClearRTV(rt, color);
    device->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto list = CreateGraphicsList(device);
    window->Run(Draw, device, list);
    DestroyGraphicsObject(list);
    DestroyGraphicsObject(device);
    DestroyRenderWindow(window);
    return 0;
}