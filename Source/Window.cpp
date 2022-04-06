#include "Window.h"
namespace Rainbow3D {
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    Window* CreateRenderWindow(const wchar_t* title, uint32_t width, uint32_t height) {
        return new Window(title, width, height);
    }

    void DestroyRenderWindow(Window* window) {
        delete window;
    }
}