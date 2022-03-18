#include "JSON.h"
#include "Debug.h"
#include <Windows.h>
namespace {
	struct UTF8_String {
		UTF8_String(const char *str,unsigned int num){}
	};

	void _readfile(const wchar_t* file) {
		auto _handle = CreateFileW(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (_handle == INVALID_HANDLE_VALUE) {
			RAINBOW3D_ERROR(L"Failed to create file.");
			RAINBOW3D_EXIT(GetLastError());
		}
		LARGE_INTEGER _para = {};
		if (GetFileSizeEx(_handle, &_para) == FALSE) {
			RAINBOW3D_ERROR(L"Failed to get file size.");
			RAINBOW3D_EXIT(GetLastError());
		}
		auto size = _para.QuadPart;
		if (size > 0xFFFFFFFF) {
			RAINBOW3D_ERROR(L"File is too large.");
			RAINBOW3D_EXIT(1);
		}
		char* ReadBuffer = new char[size]();
		DWORD dwRead = 0;
		if (FALSE == ReadFile(_handle, ReadBuffer, size, &dwRead, NULL)) {
			RAINBOW3D_ERROR(L"Failed to read file.");
			RAINBOW3D_EXIT(GetLastError());
		}

		delete[]ReadBuffer;
	}
}

bool ParseJSON(const wchar_t* file) {
	_readfile(file);
	return true;
}



