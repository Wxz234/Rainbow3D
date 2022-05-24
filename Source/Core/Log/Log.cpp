#include "Core/Log/Log.h"
#include <Windows.h>
namespace Rainbow3D {
	void OutputLogMessage(LOG_TYPE log_type, const std::string& log_str) {
		OutputDebugStringA(log_str.c_str());
	}
	void OutputLogMessage(LOG_TYPE log_type, const std::wstring& log_str) {
		OutputDebugStringW(log_str.c_str());
	}
}