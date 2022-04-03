#include <vector>
#include "Model.h"
#include "Log.h"
bool LoadFromFile(std::vector<char8_t> &modeldata,const wchar_t* filePath) {
	auto _handle = CreateFileW(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (_handle == INVALID_HANDLE_VALUE) {
		RAINBOW3D_ERROR(L"Failed to create file.");
		return false;
	}
	LARGE_INTEGER _para = {};
	if (GetFileSizeEx(_handle, &_para) == FALSE) {
		RAINBOW3D_ERROR(L"Failed to get file size.");
		CloseHandle(_handle);
		return false;
	}
	auto _size = _para.QuadPart;
	if (_size > 0xFFFFFFFF) {
		RAINBOW3D_ERROR(L"File is too large.");
		CloseHandle(_handle);
		return false;
	}
	modeldata.resize(_size);
	DWORD dwRead = 0;
	if (FALSE == ReadFile(_handle, modeldata.data(), _size, &dwRead, NULL)) {
		RAINBOW3D_ERROR(L"Failed to read file.");
		modeldata.clear();
		CloseHandle(_handle);
		return false;
	}

	CloseHandle(_handle);
	return true;
}



GLTFModel* CreateGLTFModelFromFile(const wchar_t* file) {
	std::vector<char8_t> data;
	if (!LoadFromFile(data, file)) {
		return nullptr;
	}


	return new GLTFModel();
}

void DestroyModel(Model* model) {
	delete model;
}
