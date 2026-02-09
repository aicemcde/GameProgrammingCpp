#pragma once
#include <memory>
#include <source_location>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

class Log
{
public:
	static void Init();
	static std::shared_ptr<spdlog::logger>& GetLogger();

	/// Trace
	template <typename... Args>
	static void Trace (const std::source_location& loc, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		LogInternal(spdlog::level::trace, loc, fmt, std::forward<Args>(args)...);
	}

	// Info
	template<typename... Args>
	static void Info(const std::source_location& loc, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		LogInternal(spdlog::level::info, loc, fmt, std::forward<Args>(args)...);
	}
	//Warn
	template <typename... Args>
	static void Warn(const std::source_location& loc, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		LogInternal(spdlog::level::warn, loc, fmt, std::forward<Args>(args)...);
	}

	// Error
	template <typename... Args>
	static void Error(const std::source_location& loc, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		LogInternal(spdlog::level::err, loc, fmt, std::forward<Args>(args)...);
	}

	// Critical
	template <typename... Args>
	static void Critical(const std::source_location& loc, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		LogInternal(spdlog::level::critical, loc, fmt, std::forward<Args>(args)...);
	}
private:
	static std::shared_ptr<spdlog::logger> s_Logger;

	template<typename... Args>
	static void LogInternal(spdlog::level::level_enum level, const std::source_location& loc,
		spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		spdlog::source_loc spdLogLoc
		{
			loc.file_name(),
			static_cast<int>(loc.line()),
			loc.function_name()
		};

		GetLogger()->log(spdLogLoc, level, fmt, std::forward<Args>(args)...);
	}
};

#define LOG_TRACE(...) Log::Trace(std::source_location::current(), __VA_ARGS__)
#define LOG_INFO(...) Log::Info(std::source_location::current(), __VA_ARGS__)
#define LOG_WARN(...) Log::Warn(std::source_location::current(), __VA_ARGS__)
#define LOG_ERROR(...) Log::Error(std::source_location::current(), __VA_ARGS__)
#define LOG_CRITICAL(...) Log::Critical(std::source_location::current(), __VA_ARGS__)

#include <cassert>

#define GAME_ASSERT(check, msg) \
	if(!(check)) { \
		LOG_CRITICAL("ASSERTION FAILED: {} (file : {}, line: {})", msg, __FILE__, __LINE__); \
		assert(check); \
	}