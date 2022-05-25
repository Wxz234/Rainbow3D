#include "Rainbow3D.h"

using namespace Rainbow3D;

void Draw(Device* device, SwapChain* swapchain) {

	auto context = device->GetContext();

    swapchain->Present();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device->GetDevice(), window->GetHWND(), w, h);
    auto utility = CreateUtility(device->GetDevice(), swapchain->GetSwapChain());
    utility->Init();
	return window->Run(Draw, device.Get(), swapchain.Get());
}