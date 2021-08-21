﻿#include <Nen.hpp>
#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace nen
{
	std::unique_ptr<Logger::ILogger> Logger::mLogger = nullptr;

	void Logger::MakeLogger(std::unique_ptr<ILogger> logger)
	{
		mLogger = std::move(logger);
	}

	void Logger::NenLoggers::ConsoleLogger::Debug(std::string_view string)
	{
#ifdef _DEBUG
		std::cout << "DEBUG: " << string << std::endl;
#endif
	}
	void Logger::NenLoggers::ConsoleLogger::Info(std::string_view string)
	{
#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_INFO, "NEN" , "INFO: %s\n",string.data());
#else
        std::cout << "INFO: " << string << std::endl;
#endif
	}
	void Logger::NenLoggers::ConsoleLogger::Error(std::string_view string)
	{

#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_ERROR, "NEN" , "ERROR: %s\n", string.data());

#else
        std::cout << "ERROR: " << string << std::endl;
#endif
	}
	void Logger::NenLoggers::ConsoleLogger::Warn(std::string_view string)
	{
		std::cout << "WARN: " << string << std::endl;
	}
	void Logger::NenLoggers::ConsoleLogger::Fatal(std::string_view string)
	{
		std::cout << "FATAL: " << string << std::endl;
	}

	void Logger::NenLoggers::FileLogger::Debug(std::string_view string)
	{
#ifdef _DEBUG
		std::cout << "DEBUG: " << string << std::endl;
#endif
	}
	void Logger::NenLoggers::FileLogger::Info(std::string_view string)
	{
		std::cout << "INFO: " << string << std::endl;
	}
	void Logger::NenLoggers::FileLogger::Error(std::string_view string)
	{
		std::cout << "ERROR: " << string << std::endl;
	}
	void Logger::NenLoggers::FileLogger::Warn(std::string_view string)
	{
		std::cout << "WARN: " << string << std::endl;
	}
	void Logger::NenLoggers::FileLogger::Fatal(std::string_view string)
	{
		std::cout << "FATAL: " << string << std::endl;
	}

}