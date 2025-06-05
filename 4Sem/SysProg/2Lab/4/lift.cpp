#include "lift.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <string>
#include <random>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Lift::Lift(int liftId, int liftCapacity, int maxFloorCount)
  : liftId_(liftId)
  , liftCapacity_(liftCapacity)
  , currentFloor_(0)
  , maxFloorCount_(maxFloorCount)
{}

bool Lift::isAvailable() const {
    return passengers_.empty() && rideRequests_.empty();
}

int Lift::getCurrentFloor() const {
    return currentFloor_;
}

int Lift::getLiftId() const {
    return liftId_;
}

int Lift::getCapacity() const {
    return liftCapacity_;
}

void Lift::requestRide(const Passenger& passenger) {
    g_logger->log(Logger::Level::INFO,
        string("Lift ") + to_string(liftId_) +
        ": ride request from " + to_string(passenger.from) +
        " to " + to_string(passenger.to));
    rideRequests_.push(passenger);
}

void Lift::update() {
    if (passengers_.empty() && rideRequests_.empty()) return;

    openLiftDoors();
    boardPassengers();
    closeLiftDoors();
    moveLift();
}

void Lift::openLiftDoors() {
    g_logger->log(Logger::Level::INFO,
        string("Lift ") + to_string(liftId_) +
        " opening doors at floor " + to_string(currentFloor_));
    this_thread::sleep_for(chrono::milliseconds(100));
}

void Lift::boardPassengers() {
    for (auto it = passengers_.begin(); it != passengers_.end();) {
        if (it->to == currentFloor_) {
            g_logger->log(Logger::Level::INFO, string("Lift ") + to_string(liftId_) +
                ": passenger " + to_string(it->id) + " exits at floor " + to_string(currentFloor_));
            it = passengers_.erase(it);
            ++servedPassengersCount_;
        } else ++it;
    }

    queue<Passenger> remainingRequests;
    while (!rideRequests_.empty()) {
        const auto& passenger = rideRequests_.front();
        if (passenger.from == currentFloor_ && int(passengers_.size()) < liftCapacity_) {
            passengers_.push_back(passenger);
            g_logger->log(Logger::Level::INFO,
                string("Lift ") + to_string(liftId_) +": passenger " + to_string(passenger.id) +
                " enters with destination " + to_string(passenger.to));
        } else {
            remainingRequests.push(passenger);
        }
        rideRequests_.pop();
    }
    rideRequests_ = std::move(remainingRequests);
    this_thread::sleep_for(chrono::milliseconds(100));
}

void Lift::closeLiftDoors() {
    g_logger->log(Logger::Level::INFO,
        string("Lift ") + to_string(liftId_) + " closing doors at fl00r " + to_string(currentFloor_));
    this_thread::sleep_for(chrono::milliseconds(100));
}

void Lift::moveLift() {
    int targetFloor = selectNextFloor();
    if (targetFloor == currentFloor_) return;

    this_thread::sleep_for(chrono::milliseconds(100));
    if (targetFloor > currentFloor_) {
        ++currentFloor_;
        g_logger->log(Logger::Level::INFO,
            string("Lift ") + to_string(liftId_) + " moving up to " + to_string(currentFloor_));
    } else {
        --currentFloor_;
        g_logger->log(Logger::Level::INFO,
            string("Lift ") + to_string(liftId_) + " moving down to " + to_string(currentFloor_));
    }
    ++floorsCovered_;
}

int Lift::selectNextFloor() {
    if (!passengers_.empty())
        return passengers_.front().to;
    if (!rideRequests_.empty())
        return rideRequests_.front().from;
    return currentFloor_;
}

int Lift::getTotalPassengers() const {
    return servedPassengersCount_;
}

int Lift::getTotalFloorsCovered() const {
    return floorsCovered_;
}

LiftSystem::LiftSystem(int liftCount, int maxFloorCount) : maxFloorCount_(maxFloorCount) {
    srand(time(nullptr));
    for (int i = 0; i < liftCount; ++i) {
        int randomCapacity = rand() % 7 + 2;
        lifts_.emplace_back(i, randomCapacity, maxFloorCount_);
    }
}

void LiftSystem::simulate(int timeTicks) {
    srand(time(nullptr));
    for (int t = 0; t < timeTicks; ++t) {
        int rideRequests = rand() % 5 + 1;
        for (int i = 0; i < rideRequests; ++i) {
            int fromFloor = rand() % (maxFloorCount_ + 1);
            int toFloor = rand() % (maxFloorCount_ + 1);
            Passenger passenger{ nextPassengerId_++, fromFloor, toFloor };
            getMostIdleLift().requestRide(passenger);
        }

        for (auto& lift : lifts_) {
            lift.update();
        }
    }
}


Lift& LiftSystem::getMostIdleLift() {
    return *std::min_element(
        lifts_.begin(), lifts_.end(),
        [](const Lift& a, const Lift& b) {
            return a.getCurrentFloor() < b.getCurrentFloor();
        }
    );
}

void LiftSystem::displayStatistics() const {
    int totalPassengers = 0, totalFloors = 0;
    std::cout << "System Statistics\n";
    for (const auto& lift : lifts_) {
        std::cout << "Lift" << lift.getLiftId()
                  << ": transport'd " << lift.getTotalPassengers() << " passengers, traveled " << lift.getTotalFloorsCovered()
                  << " fl00rs\n";
        totalPassengers += lift.getTotalPassengers();
        totalFloors += lift.getTotalFloorsCovered();
    }
    std::cout << "count of passengers - " << totalPassengers << "\n"
              << "count of fl00rs - " << totalFloors << "\n";
}

