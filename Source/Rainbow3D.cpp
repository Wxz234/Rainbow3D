#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
using namespace Rainbow3D;

void Draw(GraphicsDevice* device, RenderTarget* rendertarget, RenderTarget* depthtarget) {
    device->SetRenderTarget(rendertarget, depthtarget);
    float color[4] = { 0,0,1,1 };
    device->ClearRTV(rendertarget, color);
    device->ClearDSV(depthtarget, CLEAR_FLAGS::DEPTH, 1.0f, 0);
    device->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto list = CreateGraphicsList(device);
    auto rendertarget = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);
    auto depthstenciltarget = CreateRenderTarget(device, width, height, FORMAT::D32_FLOAT);
    window->Run(Draw, device, rendertarget, depthstenciltarget);
    DestroyGraphicsObject(depthstenciltarget);
    DestroyGraphicsObject(rendertarget);
    DestroyGraphicsObject(list);
    DestroyGraphicsObject(device);
    DestroyRenderWindow(window);
    return 0;
}