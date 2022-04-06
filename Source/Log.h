#pragma once
#include <Windows.h>
#include <string_view>
#include <string>
#include <format>
namespace Rainbow3D {
	template <typename... _Types>
	void RAINBOW3D_ERROR(const std::wstring_view _Fmt, const _Types&... _Args) {
		auto temp = std::wstring(L"Error:") + std::format(_Fmt, _Args...) + std::wstring(L"\n");
		OutputDebugStringW(temp.c_str());
	}
}
