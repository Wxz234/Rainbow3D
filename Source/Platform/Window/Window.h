#pragma once
#include "Core/CoreTypes.h"
#include "Core/CoreDef.h"
#include "Core/Pointer/SmartPointer.h"

namespace Rainbow3D {

	class WindowContext;

	class RWindow : public RainbowObject {
	public:
		virtual WindowContext* GetContext() = 0;
		virtual void Show() = 0;
		virtual void Dispatch() = 0;
		virtual bool IsActive() = 0;
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32_t width, uint32_t height);
}