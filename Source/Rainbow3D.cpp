#include "Rainbow3D.h"
using namespace Rainbow3D;

void Draw(GraphicsUtility* utility, RenderTarget* renderget) {
    utility->PresentRenderTarget(renderget);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.Get(), window->GetHWND(), w, h);
    auto utility = CreateGraphicsUtility(device.Get(), swapchain.Get());
    auto picture = utility->CreateRenderTargetFromFile(L"C:\\Users\\WangYuzhi\\Desktop\\x.jpg");
    auto picture2 = utility->CreateRenderTargetFromFile(L"C:\\Users\\WangYuzhi\\Desktop\\x.jpg");
    window->Show();
    window->Run(Draw, utility.Get(), picture.Get());
    return 0;
}