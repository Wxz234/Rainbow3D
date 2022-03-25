#include <Windows.h>
#include "Rainbow3D.h"
#include "Decive.h"
#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32_t width = 800, height = 600;
    Window window(L"Rainbow3D", width, height);
    Device device(window.GetHWND(), width, height);
    window.Run(
        [&device]() {
            device.Present();
        }
    );
    return 0;
}