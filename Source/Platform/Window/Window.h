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
		template <typename Function, typename... Args>
		int Run(Function&& f, Args&&... args) {
			ShowWindow(GetHWND(), SW_SHOWDEFAULT);
			MSG msg{};
			for (; msg.message != WM_QUIT;) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				std::invoke(std::forward<Function>(f), std::forward<Args>(args)...);
			}

			return static_cast<int>(msg.wParam);
		}

		virtual void Resize(uint32 width, uint32 height) = 0;
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32 width, uint32 height);
	
}