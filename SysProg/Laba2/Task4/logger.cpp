#include "logger.hpp"

std::mutex Logger::generate_log_mutex;

const char* Logger::StringLevel(LoggingLevel level) {
	switch (level) {
		case LoggingLevel::DEBUG:
			return "DEBUG";
		case LoggingLevel::INFO:
			return "INFO";
		case LoggingLevel::WARNING:
			return "WARNING";
		case LoggingLevel::ERROR:
			return "ERROR";
		case LoggingLevel::CRITICAL:
			return "CRITICAL";
		default:
			return "UNKNOWN";
	}
}

std::string Logger::LogName(const std::string& directory, LoggingLevel level) {
	std::lock_guard<std::mutex> lock(generate_log_mutex);

	if (!std::filesystem::exists(directory)) {
		std::filesystem::create_directories(directory);
	}

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm = {};

	localtime_r(&t, &tm);

	std::ostringstream buffer;
	buffer << directory << "/"  << StringLevel(level) << "_" << std::put_time(&tm, "%Y%m%d_%H%M%S");
	std::string filename = buffer.str() + ".log";

	int count = 1;
	while (std::filesystem::exists(filename)) {
		filename = buffer.str() + "(" + std::to_string(count) + ").log";
		count++;
	}

	return filename;
}

void Logger::OpenFile() {
	std::string path = LogName(log_directory, log_level);
	file.open(path, std::ios::app);

	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file: " + path);
	}
}

void Logger::Log(LoggingLevel message_level, const std::string& message) {
	if (static_cast<int>(message_level) < static_cast<int>(log_level))
		return;

	std::lock_guard<std::mutex> lock(log_mutex);

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm = {};

	localtime_r(&t, &tm);

	std::ostringstream log_message;
	log_message << '[' << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] [" << StringLevel(message_level) << "] " <<
		message << std::endl;

	file << log_message.str();
	file.flush();

	if (file.fail()) {
		throw std::runtime_error("Failed to write to log file");
	}
}