#pragma once
#include "Core/CoreTypes.h"

#include <Windows.h>
#include <functional>
#include <utility>
namespace Rainbow3D {

	template <typename Function, typename... Args>
	inline int32 RunLoop(Function&& f, Args&&... args) {
		MSG msg{};
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			std::invoke(std::forward<Function>(f), std::forward<Args>(args)...);
		}

		return static_cast<int32>(msg.wParam);
	}
}