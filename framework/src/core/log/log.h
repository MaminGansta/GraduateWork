#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

namespace Bubble
{
	class Log
	{
		static std::shared_ptr<spdlog::logger> client_logger;
		static std::shared_ptr<spdlog::logger> core_logger;

	public:
		Log() = delete;
		
		static void init();
		
		static std::shared_ptr<spdlog::logger>& GetLogger() { return client_logger; }
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return core_logger; }
	};
}

#define LOG_CORE_TRACE(...) ::Bubble::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)  ::Bubble::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)  ::Bubble::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::Bubble::Log::GetCoreLogger()->error(__VA_ARGS__)

#define LOG_TRACE(...) ::Bubble::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  ::Bubble::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::Bubble::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Bubble::Log::GetLogger()->error(__VA_ARGS__)

#ifdef _DEBUG
#define DEBUG_LOG_TRACE(...) ::Bubble::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DEBUG_LOG_INFO(...)  ::Bubble::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DEBUG_LOG_WARN(...)  ::Bubble::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DEBUG_LOG_ERROR(...) ::Bubble::Log::GetCoreLogger()->error(__VA_ARGS__)
#else
#define DEBUG_LOG_TRACE(...)
#define DEBUG_LOG_INFO(...) 
#define DEBUG_LOG_WARN(...) 
#define DEBUG_LOG_ERROR(...)
#endif
