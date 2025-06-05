#ifndef LIFT_HPP
#define LIFT_HPP

#include <vector>
#include <queue>

struct Passenger {
    int id;
    int from;
    int to;
};
class Lift {
public:
    Lift(int liftId, int liftCapacity, int maxFloorCount);

    bool isAvailable() const;
    int getCurrentFloor() const;
    int getLiftId() const;
    int getCapacity() const;
    int getTotalPassengers() const;
    int getTotalFloorsCovered() const;
    void requestRide(const Passenger& passenger);
    void update();

private:
    void openLiftDoors();
    void boardPassengers();
    void closeLiftDoors();
    void moveLift();
    int selectNextFloor();
    int liftId_;
    int liftCapacity_;
    int currentFloor_;
    int maxFloorCount_;

    std::vector<Passenger> passengers_;
    std::queue<Passenger> rideRequests_;

    int servedPassengersCount_ = 0;
    int floorsCovered_ = 0;
};

class LiftSystem {
public:
    LiftSystem(int liftCount, int maxFloorCount);
    void simulate(int timeTicks);
    void displayStatistics() const;

private:
    Lift& getMostIdleLift();
    std::vector<Lift> lifts_;
    int maxFloorCount_;
    int nextPassengerId_ = 0;
};
#endif //LIFT_HPP
