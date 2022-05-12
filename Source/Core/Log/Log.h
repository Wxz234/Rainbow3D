#pragma once

#include <string_view>
#include <string>
#include <chrono>
#include <format>

namespace Rainbow3D {

	void _outputA(const char* outputString);
	void _outputW(const wchar_t* outputString);

	template <typename... _Types>
	inline void Rainbow3D_Error(const std::string_view _Fmt, const _Types&... _Args) {
		//const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		//auto temp = std::format("[{:%F %T}] [ERROR]   ", local) + std::format(_Fmt, _Args...) + std::string("\n");
		//_outputA(temp.c_str());
	}

	template <typename... _Types>
	inline void Rainbow3D_Error(const std::wstring_view _Fmt, const _Types&... _Args) {
		//const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		//auto temp = std::format(L"[{:%F %T}] [ERROR]   ", local) + std::format(_Fmt, _Args...) + std::wstring(L"\n");
		//_outputW(temp.c_str());
	}

	template <typename... _Types>
	inline void Rainbow3D_Warning(const std::string_view _Fmt, const _Types&... _Args) {
		//const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		//auto temp = std::format("[{:%F %T}] [WARNING] ", local) + std::format(_Fmt, _Args...) + std::string("\n");
		//_outputA(temp.c_str());
	}

	template <typename... _Types>
	inline void Rainbow3D_Warning(const std::wstring_view _Fmt, const _Types&... _Args) {
		//const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		//auto temp = std::format(L"[{:%F %T}] [WARNING] ", local) + std::format(_Fmt, _Args...) + std::wstring(L"\n");
		//_outputW(temp.c_str());
	}
}
