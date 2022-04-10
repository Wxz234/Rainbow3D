#pragma once
#include <Windows.h>
#include <cstdint>
namespace Rainbow3D {
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    class Window {
    public:
        Window(const wchar_t* title, uint32_t width, uint32_t height) {
            WNDCLASSEXW wcex = {};
            wcex.cbSize = sizeof(WNDCLASSEXW);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = WndProc;
            wcex.hInstance = GetModuleHandle(nullptr);
            wcex.hIcon = LoadIconW(wcex.hInstance, L"IDI_ICON");
            wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
            wcex.lpszClassName = L"Rainbow3D";
            wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
            RegisterClassExW(&wcex);
            RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);
            hwnd = CreateWindowExW(0, L"Rainbow3D", title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
        }
        ~Window() {}
        Window(const Window&) = delete;
        Window(Window&& r) noexcept = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&& r) noexcept = delete;

        HWND GetHWND() const noexcept {
            return hwnd;
        }

        template <typename _Fn, typename ..._Args>
        void Run(_Fn&& _Fx, _Args&&... _Ax) {
            ShowWindow(hwnd, SW_SHOW);
            MSG msg = {};
            while (WM_QUIT != msg.message) {
                if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else {
                    _Fx(_Ax...);
                }
            }
        }

    private:
        HWND hwnd;
    };

    Window* CreateRenderWindow(const wchar_t* title, uint32_t width, uint32_t height);
    void DestroyRenderWindow(Window* window);
}
