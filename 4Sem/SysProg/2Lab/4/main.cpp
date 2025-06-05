#include "log.hpp"
#include "lift.hpp"
#include <memory>

int main() {
    LoggerBuilder builder;
    builder.setName("LiftSim")
           .setLevel(Logger::Level::INFO)
           .addHandler(std::make_unique<StreamLoggerHandler>(std::cout));
    auto* logger = builder.build();

    int numElevators = 4;
    int maxFloor = 11;
    int ticks = 19;

    LiftSystem system(numElevators, maxFloor);
    system.simulate(ticks);
    system.displayStatistics();

    g_logger->close();
    delete logger;
    return 0;
}