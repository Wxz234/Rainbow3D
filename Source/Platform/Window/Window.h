#pragma once
#include "Core/CoreTypes.h"
#include "Core/Pointer/UniquePtr.h"

#include <Windows.h>

namespace Rainbow3D {

	class RWindow {
	public:
		virtual ~RWindow() {}
		virtual HWND GetHWND() = 0;
		virtual void Show() = 0;
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32 width, uint32 height);
	
}