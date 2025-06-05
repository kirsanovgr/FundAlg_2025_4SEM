#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <vector>
#include "logger.hpp"

struct Passenger {
	int id;
	int from_floor;
	int to_floor;
	int generation_tick; // Время создания (чтобы потом вычислить время ожидания)
};

enum class ElevatorState {
	IDLE,
	MOVING,
	DOORS_OPENING,
	LOADING, // Посадка/высадка
	DOORS_CLOSING
};

class Elevator {
private:
	int id_;
	int current_floor_;
	int capacity_;
	std::vector<Passenger> passengers_;
	std::vector<int> target_floors_; // Целевые этажи
	ElevatorState state_;
	int ticks_remaining_; // таймер, отсчитывает количество тиков до завершения действия
	int total_passengers_;
	int total_floors_moved_;
	int direction_; // 1 - up, -1 - down, 0 - idle

public:
	Elevator(int id, int capacity);

	void update(std::vector<Passenger>& waiting_passengers, int current_tick, std::vector<int>& waiting_times,
	            Logger* logger);

	void addTargetFloor(int floor);

	[[nodiscard]] int getCurrentFloor() const;

	[[nodiscard]] int getTotalPassengers() const;

	[[nodiscard]] int getTotalFloorsMoved() const;

	[[nodiscard]] int getId() const;

	[[nodiscard]] int getCapacity() const;

	[[nodiscard]] ElevatorState getState() const;
};

#endif // ELEVATOR_HPP