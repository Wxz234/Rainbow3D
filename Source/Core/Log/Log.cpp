#include "Core/Log/Log.h"
#include <Windows.h>

namespace Rainbow3D {
	void _outputA(const char* outputString) {
		OutputDebugStringA(outputString);
	}
	void _outputW(const wchar_t* outputString) {
		OutputDebugStringW(outputString);
	}
}
