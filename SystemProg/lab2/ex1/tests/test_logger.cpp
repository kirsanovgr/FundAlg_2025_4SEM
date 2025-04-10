#include "../include/logger.hpp"


int main() {
    Logger *logger = LoggerBuilder::build("logger");
    logger->addHandler(std::make_unique<FileLoggerHandler>("test.log"));
    logger->addHandler(std::make_unique<StreamLoggerHandler>(std::cout));
    for (int i = 0; i < 100; ++i) {
        logger->Log(static_cast<Logger::LevelLogger>(rand() % 5), "asddsa");
    }
}