#include "Rainbow3D.h"

#include <Windows.h>

using namespace Rainbow3D;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32_t width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateDevice(GRAPHICS_TYPE::D3D11, window);
    auto pp = CreateModelFromFile(L"C:\\Users\\WangYuzhi\\Desktop\\glTF-Sample-Models-master\\2.0\\TriangleWithoutIndices\\glTF\\TriangleWithoutIndices.gltf");
    window->Run(
        [](Device* device) {
            float color[4] = { 1.f,0.76f,0.33f,1.f };
            device->ClearRTV(color);
            device->Present();
        },
        device
    );

    DestroyDevice(device);
    DestroyRenderWindow(window);
    return 0;
}