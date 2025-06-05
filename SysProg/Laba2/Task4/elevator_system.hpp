#ifndef ELEVATOR_SYSTEM_HPP
#define ELEVATOR_SYSTEM_HPP

#include "elevator.hpp"
#include <vector>
#include <random>

class ElevatorSystem {
private:
	int floor_count_;
	int total_ticks_;
	int current_tick_;
	std::vector<std::unique_ptr<Elevator> > elevators_;
	std::vector<Passenger> waiting_passengers_;
	std::unique_ptr<Logger> logger_;
	std::mt19937 rng_;
	int passenger_id_counter_;
	std::vector<int> waiting_times_; // времена ожидания пассажиров

	void generate_passengers();

	void assign_passengers_to_elevators();

	void update_elevators();

	void log_system_state() const;

public:
	ElevatorSystem(int floors, int elevators_count, int elevator_capacity_min, int elevator_capacity_max, int min_ticks,
	               int max_ticks);

	void run();

	void print_statistics();
};

#endif // ELEVATOR_SYSTEM_HPP