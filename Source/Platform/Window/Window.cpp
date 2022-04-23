#include "Platform/Window/Window.h"
#include <Windows.h>
namespace Rainbow3D {

	class WindowContext {
	public:
		HWND hwnd;
	};


	RWindow::RWindow(uint32 w, uint32 h) {
		_context = new WindowContext;
	}
	RWindow::~RWindow() {
		delete _context;
	}

}