#include "Debug.h"
#include <Windows.h>
#include <debugapi.h>
#include <string>
void RAINBOW3D_EXIT(unsigned int exitcode) {
	ExitProcess(exitcode);
}

void RAINBOW3D_ERROR(const wchar_t* message) {
	auto temp = std::wstring(L"Error:") + message + std::wstring(L"\n");
	OutputDebugStringW(temp.c_str());
}