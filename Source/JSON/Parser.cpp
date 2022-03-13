#include <Rainbow3D.h>
#include <Windows.h>
#include <cstdint>
namespace {
	int64_t _filesize(const wchar_t*file) {
		auto _handle = CreateFile2(file, GENERIC_READ, 0, OPEN_EXISTING, NULL);
		LARGE_INTEGER _para;
		GetFileSizeEx(_handle, &_para);
		int64_t num = _para.QuadPart;
		return num;
	}
}
