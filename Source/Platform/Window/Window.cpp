#include "Platform/Window/Window.h"
#include "Platform/Window/WindowContext.h"
#include <Windows.h>
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

	class _RWindow : public RWindow {
	public:
		_RWindow(const wchar_t* title, uint32 w, uint32 h) {
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
			RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);
			context.hwnd = CreateWindowExW(0, L"Rainbow3D", title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
			ZeroMemory(&msg, sizeof(MSG));
		}
		~_RWindow() {}
		void Show() {
			::ShowWindow(context.hwnd, SW_SHOWDEFAULT);
		}

		bool IsActive() {
			return WM_QUIT != msg.message;
		}

		void Dispatch() {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		WindowContext* GetContext() {
			return &context;
		}

		void Release() {
			delete this;
		}

		WindowContext context;
		MSG msg;
	};

	RWindow* CreateRenderWindow(const wchar_t* title, uint32_t width, uint32_t height) {
		return new _RWindow(title, width, height);
	}

}