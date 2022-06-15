#pragma once
#include "Core/CorePreDef.h"
namespace Rainbow3D {

	template <typename ...T>
	inline void RainbowLog(const std::string_view log, T&&... _Args) {
		std::string type_str = " [INFO] ";
		const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		std::string log_str = std::vformat("[{:%H:%M:%OS}]", std::make_format_args(local));
		log_str += type_str;
		log_str += std::vformat(log, std::make_format_args(_Args...));
		log_str += "\n";
		OutputDebugStringA(log_str.c_str());
	}

	template <typename ...T>
	inline void RainbowLog(const std::wstring_view log, T&&... _Args) {
		std::wstring type_str = L" [INFO] ";
		const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		std::wstring log_str = std::vformat(L"[{:%H:%M:%OS}]", std::make_wformat_args(local));
		log_str += type_str;
		log_str += std::vformat(log, std::make_wformat_args(_Args...));
		log_str += L"\n";
		OutputDebugStringW(log_str.c_str());
	}
}