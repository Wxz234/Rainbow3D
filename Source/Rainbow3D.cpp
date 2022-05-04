#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
#include <thread>
using namespace Rainbow3D;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto omColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);
    device->BindRenderTarget(omColor);

    auto baseColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);
    auto baseList = CreateGraphicsList(device);

    window->Show();
    while (window->IsActive()) {
        window->Dispatch();
        baseList->Open();
        float outputColor[4] = { 1.f,0.2f,0.5f,1.f };
        baseList->ClearRTV(omColor, outputColor);
        baseList->Close();
        device->ExecuteCommandList(baseList);
        device->Present();
    }

    baseList->Release();
    baseColor->Release();
    omColor->Release();
    device->Release();
    window->Release();
    return 0;
}