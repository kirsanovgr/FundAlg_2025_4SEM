#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <fstream>


class LogHandler {
public:
    virtual void write(const std::string &message) = 0;
    virtual ~LogHandler() = default;
};

class StreamLoggerHandler : public LogHandler {
protected:
    std::ostream &stream_;
public:
    explicit StreamLoggerHandler(std::ostream &stream = std::cout);
    void write(const std::string &message) override;
};

class FileLoggerHandler : public LogHandler {
protected:
    std::ofstream out;
public:
    explicit FileLoggerHandler(const std::string & filename);
    void write(const std::string& message) override;
    ~FileLoggerHandler() override;
};

class Logger {
public:
    enum LevelLogger {
        LOG_CRITICAL,
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO,
        LOG_DEBUG
    };
private:
    std::string logger_name;
    std::vector<std::unique_ptr<LogHandler>> handlers;
    LevelLogger log_level;
    static std::string get_datetime();
public:
    Logger(std::string  logger_n, Logger::LevelLogger logger_level = Logger::LOG_DEBUG);
    void Log(LevelLogger level, const std::string &log);
    void LogError(const std::string &log);
    void LogCritic(const std::string &log);
    void LogWarning(const std::string &log);
    void LogInfo(const std::string &log);
    void LogDebug(const std::string &log);

    void addHandler(std::unique_ptr<LogHandler> handler);
    void setHandler(LevelLogger level);

    void close_logger();
};


class LoggerBuilder {
public:
    static Logger *build(const std::string &logger_name, Logger::LevelLogger logger_level = Logger::LOG_DEBUG);
};