#include <Windows.h>
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
		m_binaryData.clear();
	}
	CloseHandle(_handle);
}

bool BinaryBlob::IsEmpty() const noexcept {
	return m_binaryData.empty();
}



namespace {
	enum class UTF8_STATE
	{
		START,
		LEAD_2_1,
		LEAD_3_1_E0,
		LEAD_3_2_E0,
		LEAD_3_1_E1,
		LEAD_3_2_E1,
		LEAD_3_1_ED,
		LEAD_3_2_ED,
		LEAD_3_1_EE,
		LEAD_3_2_EE,
		LEAD_4_1_F0,
		LEAD_4_2_F0,
		LEAD_4_1_F1,
		LEAD_4_2_F1,
		LEAD_4_1_F4,
		LEAD_4_2_F4,
		LEAD_4_3,
		NOT_UTF8
	};
}

bool BinaryBlob::IsUTF8() const noexcept {

	UTF8_STATE state = UTF8_STATE::START;
	size_t str_size = m_binaryData.size();
	for (size_t i = 0;i < str_size; ++i) {
		if (state == UTF8_STATE::START) {
			if (m_binaryData[i] >= 0x00 && m_binaryData[i] <= 0x7F) {
				continue;
			}
			else if (m_binaryData[i] >= 0xC2 && m_binaryData[i] <= 0xDF) {
				state = UTF8_STATE::LEAD_2_1;
				continue;
			}
			else if (m_binaryData[i] == 0xE0) {
				state = UTF8_STATE::LEAD_3_1_E0;
				continue;
			}
			else if (m_binaryData[i] >= 0xE1 && m_binaryData[i] <= 0xEC) {
				state = UTF8_STATE::LEAD_3_1_E1;
				continue;
			}
			else if (m_binaryData[i] == 0xED) {
				state = UTF8_STATE::LEAD_3_1_ED;
				continue;
			}
			else if (m_binaryData[i] >= 0xEE && m_binaryData[i] <= 0xEF) {
				state = UTF8_STATE::LEAD_3_1_EE;
				continue;
			}
			else if (m_binaryData[i] == 0xF0) {
				state = UTF8_STATE::LEAD_4_1_F0;
				continue;
			}
			else if (m_binaryData[i] >= 0xF1 && m_binaryData[i] <= 0xF3) {
				state = UTF8_STATE::LEAD_4_1_F1;
				continue;
			}
			else if (m_binaryData[i] == 0xF4) {
				state = UTF8_STATE::LEAD_4_1_F4;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_2_1) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::START;
				continue;
			}
			else{
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_3_1_E0) {
			if (m_binaryData[i] >= 0xA0 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::LEAD_3_2_E0;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_3_1_E1) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::LEAD_3_2_E1;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_3_1_ED) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0x9F) {
				state = UTF8_STATE::LEAD_3_2_ED;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_3_1_EE) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::LEAD_3_2_EE;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_3_2_E0 || state == UTF8_STATE::LEAD_3_2_E1 || state == UTF8_STATE::LEAD_3_2_ED || state == UTF8_STATE::LEAD_3_2_EE) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::START;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_4_1_F0) {
			if (m_binaryData[i] >= 0x90 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::LEAD_4_2_F0;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_4_1_F1) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::LEAD_4_2_F1;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_4_1_F4) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0x8F) {
				state = UTF8_STATE::LEAD_4_2_F4;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_4_2_F0 || state == UTF8_STATE::LEAD_4_2_F1 || state == UTF8_STATE::LEAD_4_2_F4) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::LEAD_4_3;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else if (state == UTF8_STATE::LEAD_4_3) {
			if (m_binaryData[i] >= 0x80 && m_binaryData[i] <= 0xBF) {
				state = UTF8_STATE::START;
				continue;
			}
			else {
				state = UTF8_STATE::NOT_UTF8;
				continue;
			}
		}
		else
			break;
		
	}

	return state == UTF8_STATE::START;
}