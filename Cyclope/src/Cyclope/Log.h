#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"

namespace Cyclope {

	class CYCLOPE_API Log {

	public:
		static void Init();
		static inline Shared<spdlog::logger> GetCoreLogger() { return s_core_logger; }
		static inline Shared<spdlog::logger> GetClientLogger() { return s_client_logger; }

	private:
		static Shared<spdlog::logger> s_core_logger;
		static Shared<spdlog::logger> s_client_logger;

	};

}

#define CYCLOPE_CORE_TRACE(...) Cyclope::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CYCLOPE_CORE_INFO(...) Cyclope::Log::GetCoreLogger()->info(__VA_ARGS__)	 
#define CYCLOPE_CORE_WARN(...) Cyclope::Log::GetCoreLogger()->warn(__VA_ARGS__)	 
#define CYCLOPE_CORE_ERROR(...) Cyclope::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CYCLOPE_CORE_CRITICAL(...) Cyclope::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define CYCLOPE_TRACE(...) Cyclope::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CYCLOPE_INFO(...) Cyclope::Log::GetClientLogger()->info(__VA_ARGS__)	 
#define CYCLOPE_WARN(...) Cyclope::Log::GetClientLogger()->warn(__VA_ARGS__)	 
#define CYCLOPE_ERROR(...) Cyclope::Log::GetClientLogger()->error(__VA_ARGS__)
#define CYCLOPE_CRITICAL(...) Cyclope::Log::GetClientLogger()->critical(__VA_ARGS__)
