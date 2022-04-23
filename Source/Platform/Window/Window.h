#pragma once
#include "Core/CoreTypes.h"
#include "Core/CoreDef.h"
namespace Rainbow3D {

	class WindowContext;

	class RWindow final {
	public:
		DISABLE_COPY_AND_ASSIGN(RWindow)

		RWindow(uint32 w, uint32 h);
		~RWindow();

		WindowContext* GetContext() const noexcept {
			return _context;
		}
	private:
		WindowContext *_context;
	};
}