#include "Platform/Window/Window.h"
#include "ThirdParty/imgui/imgui.h"
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

	class RenderWindow : public RWindow {
	public:
		RenderWindow(const wchar_t* title, uint32 width, uint32 height) {
			ZeroMemory(&msg, sizeof(MSG));
			isActive = false;			
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
		virtual void Show() {
			ShowWindow(hwnd, SW_SHOWDEFAULT);
			isActive = true;
		}
		virtual void Hide() {
			ShowWindow(hwnd, SW_HIDE);
			isActive = false;
		}
		virtual void Dispatch() {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		WindowContext GetContext() {
			WindowContext _context;
			_context.context = hwnd;
			return _context;
		}

		virtual bool IsActive() { return (WM_QUIT != msg.message) && isActive; }

		bool isActive;
		MSG msg;
		HWND hwnd;
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32 width, uint32 height) {
		return UniquePtr<RWindow>(new RenderWindow(title,width,height));
	}
}