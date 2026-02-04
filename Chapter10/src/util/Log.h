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
	// Info
	template<typename... Args>
	static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args,
		const std::source_location& loc = std::source_location::current())
	{
		LogInternal(spdlog::level::info, loc, fmt, std::forward<Args>(args)...);
	}
	//Warn
	template <typename... Args>
	static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args,
		const std::source_location& loc = std::source_location::current())
	{
		LogInternal(spdlog::level::warn, loc, fmt, std::forward<Args>(args)...);
	}

	// Error
	template <typename... Args>
	static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args,
		const std::source_location& loc = std::source_location::current())
	{
		LogInternal(spdlog::level::err, loc, fmt, std::forward<Args>(args)...);
	}

	// Critical
	template <typename... Args>
	static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args,
		const std::source_location& loc = std::source_location::current())
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

#define LOG_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)

#include <cassert>

#define GAME_ASSERT(check, msg) \
	if(!(check)) { \
		LOG_CRITICAL("ASSERTION FAILED: {} (file : {}, line: {})", msg, __FILE__, __LINE__); \
		assert(check); \
	}