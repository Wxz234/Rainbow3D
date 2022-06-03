#include "Platform/Window/Window.h"
#include "Platform/Window/Proc.h"
#include "Core/Log/Log.h"
#define DEFAULT_WIDTH 50
#define DEFAULT_HEIGHT 50

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
			stype = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
			AdjustWindowRect(&rc, stype, FALSE);
			hwnd = CreateWindowExW(0, L"Rainbow3D", title, stype, DEFAULT_WIDTH, DEFAULT_HEIGHT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
			w = width;
			h = height;
		}

	    HWND GetHWND() {
			return hwnd;
		}
		void Show() {
			ShowWindow(hwnd, SW_SHOWDEFAULT);
		}

		//void Resize(uint32 width, uint32 height) {
		//	w = width;
		//	h = height;
		//	RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
		//	AdjustWindowRect(&rc, stype, FALSE);
		//	SetWindowPos(hwnd, HWND_TOP, DEFAULT_WIDTH, DEFAULT_HEIGHT, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		//}

		HWND hwnd;

		uint32 w;
		uint32 h;

		DWORD stype;
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32 width, uint32 height) {
		return UniquePtr<RWindow>(new RenderWindow(title, width, height));
	}
}