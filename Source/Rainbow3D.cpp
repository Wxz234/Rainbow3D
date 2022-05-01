#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
using namespace Rainbow3D;

void Draw(GraphcisDevice* device, GBuffer* gbuffer) {
    ID3D11Device* pDevice = reinterpret_cast<ID3D11Device*>(device->GetNativeDevice());
    ID3D11DeviceContext* pContext = reinterpret_cast<ID3D11DeviceContext*>(device->GetNativeDeviceContext());
    
    float color[4] = { 0.3f,0.5f,0.7f,1.f };
    device->ClearRTV(color);
    device->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphcisDevice(window->GetContext(), width, height);
    auto gbuffer = CreateGBuffer(device);
    window->Run(Draw, device, gbuffer);
    DestroyGBuffer(gbuffer);
    DestroyGraphcisDevice(device);
    DestroyRenderWindow(window);
    return 0;
}