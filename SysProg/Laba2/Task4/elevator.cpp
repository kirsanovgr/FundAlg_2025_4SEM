#include "elevator.hpp"

Elevator::Elevator(int id, int capacity)
	: id_(id), current_floor_(1), capacity_(capacity), total_passengers_(0), total_floors_moved_(0),
	  state_(ElevatorState::IDLE), ticks_remaining_(0), direction_(0) {}

void Elevator::addTargetFloor(int floor) {
	if (std::find(target_floors_.begin(), target_floors_.end(), floor) == target_floors_.end()) {
		target_floors_.push_back(floor);
	}
}

void Elevator::update(std::vector<Passenger>& waiting_passengers, int current_tick, std::vector<int>& waiting_times,
                      Logger* logger) {
	if (ticks_remaining_ > 0) {
		ticks_remaining_--;
		if (ticks_remaining_ == 0) {
			switch (state_) {
				case ElevatorState::DOORS_OPENING:
					state_ = ElevatorState::LOADING;
					ticks_remaining_ = 1;
					if (logger) {
						std::ostringstream oss;
						oss << "[TICK " << current_tick << "] Elevator " << id_ << " doors opened";
						logger->LogInfo(oss.str());
					}
					break;
				case ElevatorState::LOADING: {
					std::vector<Passenger> remaining;
					for (auto& p : waiting_passengers) {
						if (p.from_floor == current_floor_ && passengers_.size() < capacity_) {
							passengers_.push_back(p);
							total_passengers_++;
							waiting_times.push_back(current_tick - p.generation_tick);
							if (logger) {
								std::ostringstream oss;
								oss << "[TICK " << current_tick << "] Passenger " << p.id << " boarded elevator " <<
									id_;
								logger->LogInfo(oss.str());
							}
						} else {
							remaining.push_back(p);
						}
					}
					waiting_passengers = remaining;
				}
					state_ = ElevatorState::DOORS_CLOSING;
					ticks_remaining_ = 1;
					break;
				case ElevatorState::DOORS_CLOSING:
					state_ = ElevatorState::IDLE;
					if (logger) {
						std::ostringstream oss;
						oss << "[TICK " << current_tick << "] Elevator " << id_ << " doors closed";
						logger->LogInfo(oss.str());
					}
					break;
				case ElevatorState::MOVING:
					total_floors_moved_ += 1;
					current_floor_ += direction_;
					if (logger) {
						std::ostringstream oss;
						oss << "[TICK " << current_tick << "] Elevator " << id_ << " moved to floor " << current_floor_;
						logger->LogInfo(oss.str());
					}

					if (!target_floors_.empty()) {
						int next_floor = target_floors_.front();
						if (current_floor_ == next_floor) {
							// Удаление пассажиров, которые доехали
							std::vector<Passenger> remaining;
							for (const auto& p : passengers_) {
								if (p.to_floor != current_floor_) {
									remaining.push_back(p);
								}
							}
							passengers_ = remaining;

							target_floors_.erase(target_floors_.begin());
							direction_ = 0;
							state_ = ElevatorState::IDLE;
							if (!target_floors_.empty()) {
								state_ = ElevatorState::DOORS_OPENING;
								ticks_remaining_ = 1;
							}
						} else {
							ticks_remaining_ = 2;
						}
					}
					break;
				default:
					break;
			}
		}
	} else {
		if (state_ == ElevatorState::IDLE) {
			if (!target_floors_.empty()) {
				int next_floor = target_floors_.front();
				int delta = next_floor - current_floor_;
				if (delta != 0) {
					direction_ = (delta > 0) ? 1 : -1;
					state_ = ElevatorState::MOVING;
					ticks_remaining_ = 2;
					if (logger) {
						std::ostringstream oss;
						oss << "[TICK " << current_tick << "] Elevator " << id_ << " started moving to floor " <<
							next_floor;
						logger->LogInfo(oss.str());
					}
				} else {
					target_floors_.erase(target_floors_.begin());
					state_ = ElevatorState::DOORS_OPENING;
					ticks_remaining_ = 1;
				}
			}
		}
	}
}

int Elevator::getCurrentFloor() const {
	return current_floor_;
}

int Elevator::getTotalPassengers() const {
	return total_passengers_;
}

int Elevator::getTotalFloorsMoved() const {
	return total_floors_moved_;
}

int Elevator::getId() const {
	return id_;
}

ElevatorState Elevator::getState() const {
	return state_;
}

int Elevator::getCapacity() const {
	return capacity_;
}