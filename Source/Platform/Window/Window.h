#pragma once
#include "Core/Pointer/UniquePtr.h"
#include "RObject/RObject.h"
namespace Rainbow3D {
	class RWindow : public RObject {
	public:
		//virtual WindowContext* GetContext() = 0;
		virtual void Show() = 0;
		virtual void Hide() = 0;
		virtual void Dispatch() = 0;
		virtual bool IsActive() = 0;
	};

	UniquePtr<RWindow> CreateRenderWindow(const wchar_t* title, uint32_t width, uint32_t height);
}