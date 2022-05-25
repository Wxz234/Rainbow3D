#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"

#include <Windows.h>
#include <functional>
#include <utility>
namespace Rainbow3D {

	class RWindow {
	public:

		virtual ~RWindow() {}
		virtual HWND GetHWND() = 0;
		void Show() {
			ShowWindow(GetHWND(), SW_SHOWDEFAULT);
		}
		void Hide() {
			ShowWindow(GetHWND(), SW_HIDE);
		}
		template <typename Function, typename... Args>
		void Run(Function&& f, Args&&... args) {
			for (MSG msg{}; msg.message != WM_QUIT;) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				std::invoke(std::forward<Function>(f), std::forward<Args>(args)...);
			}
		}
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32 width, uint32 height);
	
}