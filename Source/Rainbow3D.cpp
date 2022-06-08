#include "Rainbow3D.h"

using namespace Rainbow3D;

void Draw(SwapChain* swapchain, GBuffer* gbuffer, ToneMapping* tonemapping) {
    
    tonemapping->Render(gbuffer->GetBaseColorBuffer(), gbuffer->GetBaseColorSRV(), swapchain->GetBuffer(), swapchain->GetRTV());
    swapchain->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.Get(), window.Get(), w, h);
    auto gbuffer = CreateGBuffer(device.Get(), w, h);
    auto tonemapping = CreateToneMapping(device.Get());

    window->Show();
    return RunLoop(Draw, swapchain.Get(), gbuffer.Get(), tonemapping.Get());
}