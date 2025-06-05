#ifndef TASK1_HPP
#define TASK1_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <mutex>

enum class LoggingLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

class Logger {
	static std::mutex generate_log_mutex;
	std::mutex log_mutex;
	std::string log_directory;
	LoggingLevel log_level;
	std::ofstream file;

	Logger(LoggingLevel level, const std::string& dir)
		: log_level(level), log_directory(dir) {
		OpenFile();
	}

	static const char* StringLevel(LoggingLevel level);

	static std::string LogName(const std::string& directory, LoggingLevel level);

	void OpenFile();

public:
	class Builder {
		LoggingLevel level = LoggingLevel::WARNING;
		std::string directory = "./logs";

	public:
		Builder& SetLoggingLevel(LoggingLevel lvl) {
			level = lvl;
			return *this;
		}

		Builder& SetDirectory(const std::string& dir) {
			directory = dir;
			return *this;
		}

		std::unique_ptr<Logger> Build() {
			return std::unique_ptr<Logger>(
				new Logger(level, directory)
				);
		}
	};

	~Logger() { file.close(); }

	void Log(LoggingLevel message_level, const std::string& message);

	void LogDebug(const std::string& message) { Log(LoggingLevel::DEBUG, message); }
	void LogInfo(const std::string& message) { Log(LoggingLevel::INFO, message); }
	void LogWarning(const std::string& message) { Log(LoggingLevel::WARNING, message); }
	void LogError(const std::string& message) { Log(LoggingLevel::ERROR, message); }
	void LogCritical(const std::string& message) { Log(LoggingLevel::CRITICAL, message); }
};

#endif //TASK1_HPP