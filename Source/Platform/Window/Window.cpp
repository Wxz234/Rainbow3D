#include "Platform/Window/Window.h"
#include <Windows.h>
namespace Rainbow3D {

	class WindowContext {
	public:
		HWND hwnd = NULL;
	};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	RWindow::RWindow(const wchar_t* title,uint32 w, uint32 h) {
		_context = new WindowContext;
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
		RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);
		_context->hwnd = CreateWindowExW(0, L"Rainbow3D", title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	}
	RWindow::~RWindow() {
		delete _context;
	}

	void RWindow::_show() {
		ShowWindow(_context->hwnd, SW_SHOW);
	}

	void* RWindow::_get_msg() {
		MSG* p_msg = new MSG{};
		return reinterpret_cast<void*>(p_msg);
	}
	bool RWindow::_is_quit(void* msg) {
		MSG* p_msg = reinterpret_cast<MSG*>(msg);
		return WM_QUIT == p_msg->message;
	}
	bool RWindow::_peek_msg(void* msg) {
		MSG* p_msg = reinterpret_cast<MSG*>(msg);
		return PeekMessage(p_msg, nullptr, 0, 0, PM_REMOVE) == TRUE;
	}
	void RWindow::_dispatch_msg(void* msg) {
		MSG* p_msg = reinterpret_cast<MSG*>(msg);
		TranslateMessage(p_msg);
		DispatchMessage(p_msg);
	}
	void RWindow::_destroy_msg(void* msg) {
		MSG* p_msg = reinterpret_cast<MSG*>(msg);
		delete p_msg;
	}

	RWindow* CreateRenderWindow(const wchar_t* title, uint32_t width, uint32_t height) {
		return new RWindow(title, width, height);
	}
	void DestroyRenderWindow(RWindow* window) {
		delete window;
	}

}