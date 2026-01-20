#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <vector>

const size_t MAX_FILE_SIZE = 1024 * 1024 * 5;
const size_t MAX_FILE_NUM = 3;

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init()
{
	std::vector<spdlog::sink_ptr> logSinks;

	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	consoleSink->set_pattern("%^[%T] [%l] [%s:%#] %n: %v%$");
	consoleSink->set_level(spdlog::level::warn);
	logSinks.emplace_back(consoleSink);

	auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/engine.log", MAX_FILE_SIZE, MAX_FILE_NUM, true);
	fileSink->set_pattern("[%T] [%l] [%s:%#] %n: %v");
	fileSink->set_level(spdlog::level::trace);
	logSinks.emplace_back(fileSink);

	s_Logger = std::make_shared<spdlog::logger>("GAME", begin(logSinks), end(logSinks));
	spdlog::register_logger(s_Logger);

	s_Logger->flush_on(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Log::GetLogger()
{
	return s_Logger;
}