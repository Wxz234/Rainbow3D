//#include <Windows.h>
//#include <cstdint>
//#include "Debug.h"
//namespace {
//
//	bool _isUTF8(char* str, uint32_t size) {
//
//		for (uint32_t i = 0;i < size; ++i) {
//
//		}
//
//		return true;
//	}
//}
//
//bool ParseJSON(const wchar_t* file) {
//	//read file
//	auto _handle = CreateFileW(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (_handle == INVALID_HANDLE_VALUE) {
//		RAINBOW3D_ERROR(L"Failed to create file.");
//		return false;
//	}
//	LARGE_INTEGER _para = {};
//	if (GetFileSizeEx(_handle, &_para) == FALSE) {
//		RAINBOW3D_ERROR(L"Failed to get file size.");
//		CloseHandle(_handle);
//		return false;
//	}
//	auto size = _para.QuadPart;
//	if (size > 0xFFFFFFFF) {
//		RAINBOW3D_ERROR(L"File is too large.");
//		CloseHandle(_handle);
//		return false;
//	}
//	char* _readBuffer = new char[size]();
//	DWORD dwRead = 0;
//	if (FALSE == ReadFile(_handle, _readBuffer, size, &dwRead, NULL)) {
//		RAINBOW3D_ERROR(L"Failed to read file.");
//		CloseHandle(_handle);
//		return false;
//	}
//
//
//	return true;
//}



