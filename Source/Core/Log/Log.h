#pragma once
#include <format>
#include <string>
#include <string_view>
#include <chrono>
namespace Rainbow3D {

	namespace LogVerbosity {
		enum Type {
			Info = 0,
			Error = 1,
			Warning = 2,
		};
	}

	void OutputLogMessage(LogVerbosity::Type log_type, const std::string& log_str);
	void OutputLogMessage(LogVerbosity::Type log_type, const std::wstring& log_str);
	template <typename ...T>
	inline void RAINBOW_LOG(LogVerbosity::Type log_type, const std::string_view log, T&&... _Args) {
		std::string type_str;
		if (log_type == LogVerbosity::Info) {
			type_str = " INFO ";
		}
		else if (log_type == LogVerbosity::Error) {
			type_str = " ERROR ";
		}
		else if (log_type == LogVerbosity::Warning) {
			type_str = " WARNING ";
		}
		else  {
			type_str = " UNKNOWN ";
		}

		const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		std::string log_str = std::vformat("[{:%H:%M:%OS}]", std::make_format_args(local));
		log_str += type_str;
		log_str += std::vformat(log, std::make_format_args(_Args...));
		log_str += "\n";
		OutputLogMessage(log_type, log_str);
	}

	template <typename ...T>
	inline void RAINBOW_LOG(LogVerbosity::Type log_type, const std::wstring_view log, T&&... _Args) {
		std::wstring type_str;
		if (log_type == LogVerbosity::Info) {
			type_str = L" INFO ";
		}
		else if (log_type == LogVerbosity::Error) {
			type_str = L" ERROR ";
		}
		else if (log_type == LogVerbosity::Warning) {
			type_str = L" WARNING ";
		}
		else {
			type_str = L" UNKNOWN ";
		}

		const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
		std::wstring log_str = std::vformat(L"[{:%H:%M:%OS}]", std::make_wformat_args(local));
		log_str += type_str;
		log_str += std::vformat(log, std::make_wformat_args(_Args...));
		log_str += L"\n";
		OutputLogMessage(log_type, log_str);
	}
}