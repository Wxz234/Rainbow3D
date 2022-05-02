#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
using namespace Rainbow3D;

void Draw(GraphicsDevice* device, GBuffer* gbuffer) {
    //ID3D11Device* pDevice = reinterpret_cast<ID3D11Device*>(device->GetNativeDevice());
    //ID3D11DeviceContext* pContext = reinterpret_cast<ID3D11DeviceContext*>(device->GetNativeDeviceContext());
    
    float color[4] = { 0.3f,0.5f,0.7f,1.f };
    device->ClearRTV(color);
    device->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);

    Mesh mesh;
    mesh.vertices = { {0.0f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f} };
    mesh.triangles = { 0,1,2 };
    auto meshrenderer = CreateMeshRenderer(device, &mesh);
    window->Run(Draw, device, nullptr);
    DestroyRenderer(meshrenderer);
    DestroyGraphicsDevice(device);
    DestroyRenderWindow(window);
    return 0;
}