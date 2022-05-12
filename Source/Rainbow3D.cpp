#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>

using namespace Rainbow3D;


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto omColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);

    auto baseColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);
    auto baseList = CreateCommandList(device);
    auto texture = CreateTexture2DFromFile(device, L"C:\\Users\\WangYuzhi\\Desktop\\R-C.jpg");
    auto deleter = [](int* p) {
        int* temp = reinterpret_cast<int*>(p);
        delete temp;
        Rainbow3D_Error("Delete sssssadas");
        Rainbow3D_Warning("Delete sssssadas");
    };

    UniquePtr<int> up6d(new int); // D is copied


    window->Show();
    while (window->IsActive()) {
        window->Dispatch();
        baseList->Open();
        float outputColor[4] = { 1.f,0.2f,0.5f,1.f };
        baseList->ClearRTV(omColor, outputColor);
        baseList->Close();
        device->ExecuteCommandList(baseList);
        device->Present(texture);
    }
    DestroyRainbowObject(texture);
    DestroyRainbowObject(baseList);
    DestroyRainbowObject(baseColor);
    DestroyRainbowObject(omColor);
    DestroyRainbowObject(device);
    DestroyRainbowObject(window);

    return 0;
}