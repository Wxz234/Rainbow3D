#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
using namespace Rainbow3D;

void Draw(GraphicsDevice* device, GraphicsList* list) {
    list->Close();
    device->ExecuteCommandList(list);
    float color[4] = { 0.3f,0.5f,0.7f,1.f };
    device->ClearRTV(color);
    device->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto list = CreateGraphicsList(device);
    window->Run(Draw, device, list);
    DestroyGraphicsList(list);
    DestroyGraphicsDevice(device);
    DestroyRenderWindow(window);
    return 0;
}