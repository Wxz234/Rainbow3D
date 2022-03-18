#include <Windows.h>
#include <cstdint>
#include "Debug.h"
#include "Blob.h"

void BinaryBlob::LoadFromFile(const wchar_t* filePath) {
	auto _handle = CreateFileW(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (_handle == INVALID_HANDLE_VALUE) {
		RAINBOW3D_ERROR(L"Failed to create file.");
		return;
	}
	LARGE_INTEGER _para = {};
	if (GetFileSizeEx(_handle, &_para) == FALSE) {
		RAINBOW3D_ERROR(L"Failed to get file size.");
		CloseHandle(_handle);
		return;
	}
	auto _size = _para.QuadPart;
	if (_size > 0xFFFFFFFF) {
		RAINBOW3D_ERROR(L"File is too large.");
		CloseHandle(_handle);
		return;
	}
	m_binaryData.resize(_size);
	DWORD dwRead = 0;
	if (FALSE == ReadFile(_handle, m_binaryData.data(), _size, &dwRead, NULL)) {
		RAINBOW3D_ERROR(L"Failed to read file.");
		CloseHandle(_handle);
		m_binaryData.clear();
	}
}

bool BinaryBlob::IsEmpty() const noexcept {
	return m_binaryData.empty();
}

bool BinaryBlob::IsUTF8() const noexcept {

	for (size_t i = 0;i < m_binaryData.size(); ++i) {
		auto code_num = std::to_integer<std::uint8_t>(m_binaryData[i]);
		if (code_num >= 0x00 && code_num <= 0x7F) {
			continue;
		}
		else if(code_num >= 0xC2 && code_num <= 0xDF) {
			
		}

	}
	return true;
}