#include "Platform/Window/Window.h"
#include "Platform/Window/Proc.h"
namespace Rainbow3D {

	class RenderWindow : public RWindow {
	public:
		RenderWindow(const wchar_t* title, uint32 width, uint32 height) {
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
			DWORD stype = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
			AdjustWindowRect(&rc, stype, FALSE);
			hwnd = CreateWindowExW(0, L"Rainbow3D", title, stype, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
		}

	    HWND GetHWND() {
			return hwnd;
		}

		HWND hwnd;
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32 width, uint32 height) {
		return UniquePtr<RWindow>(new RenderWindow(title, width, height));
	}
}