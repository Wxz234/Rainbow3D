#pragma once
#include <Windows.h>
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace Rainbow3D {

	inline LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

        case WM_SYSKEYDOWN:
            if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
            {
				
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
				//ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }
            break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}