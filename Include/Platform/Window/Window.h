#pragma once
#include "Core/CorePreDef.h"
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"

namespace Rainbow3D {

	class RWindow {
	public:
		RWindow(const wchar_t* title, uint32 width, uint32 height, bool ispopup) {
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
			if (ispopup) stype = WS_POPUP;
			else stype = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
			AdjustWindowRect(&rc, stype, FALSE);
			hwnd = CreateWindowExW(0, L"Rainbow3D", title, stype, 50, 50, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
			w = width;
			h = height;
		}

		HWND GetHWND() {
			return hwnd;
		}
		void Show() {
			ShowWindow(hwnd, SW_SHOWDEFAULT);
		}

		template <typename Function, typename... Args>
		int RunLoop(Function&& f, Args&&... args) {
			MSG msg{};
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				std::invoke(std::forward<Function>(f), std::forward<Args>(args)...);
			}

			return static_cast<int>(msg.wParam);
		}
	private:
		HWND hwnd;
		uint32 w;
		uint32 h;
		DWORD stype;
	};

	inline UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32 width, uint32 height,bool ispopup = false) {
		return UniquePtr<RWindow>(new RWindow(title, width, height, ispopup));
	}
	
}