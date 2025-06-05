#include "elevator_system.hpp"
#include <unordered_map>

ElevatorSystem::ElevatorSystem(int floors, int elevators_count, int elevator_capacity_min, int elevator_capacity_max,
                               int min_ticks, int max_ticks)
	: floor_count_(floors), current_tick_(0), passenger_id_counter_(0) {
	// Генерация случайного количества тиков
	std::random_device rd;
	rng_ = std::mt19937(rd());
	std::uniform_int_distribution<int> dist_ticks(min_ticks, max_ticks);
	total_ticks_ = dist_ticks(rng_);

	// Инициализация логгера
	logger_ = Logger::Builder().SetLoggingLevel(LoggingLevel::INFO).SetDirectory("./logs").Build();

	// Создание лифтов
	std::uniform_int_distribution<int> dist_capacity(elevator_capacity_min, elevator_capacity_max);
	for (int i = 1; i <= elevators_count; ++i) {
		int capacity = dist_capacity(rng_);
		elevators_.push_back(std::make_unique<Elevator>(i, capacity));
		if (logger_) {
			std::ostringstream oss;
			oss << "[TICK 0] Elevator " << (i) << " created with capacity: " << capacity;
			logger_->LogInfo(oss.str());
		}
	}
}

void ElevatorSystem::run() {
	for (current_tick_ = 0; current_tick_ < total_ticks_; ++current_tick_) {
		generate_passengers();
		assign_passengers_to_elevators();
		update_elevators();
		log_system_state();
	}
	print_statistics();
}

void ElevatorSystem::generate_passengers() {
	// Генерация случайного количества пассажиров
	std::uniform_int_distribution<int> dist_passengers(0, 3);
	int num_passengers = dist_passengers(rng_);

	for (int i = 0; i < num_passengers; ++i) {
		// Генерация этажей отправления и назначения
		std::uniform_int_distribution<int> dist_floor(1, floor_count_);
		int from = dist_floor(rng_);
		int to = dist_floor(rng_);
		while (from == to) {
			to = dist_floor(rng_);
		}
		Passenger p{};
		p.id = ++passenger_id_counter_;
		p.from_floor = from;
		p.to_floor = to;
		p.generation_tick = current_tick_;
		waiting_passengers_.push_back(p);

		// Логирование генерации пассажира
		std::ostringstream oss;
		oss << "[TICK " << current_tick_ << "] New passenger " << p.id << " generated. From floor " << from <<
			" to floor " << to;
		logger_->LogInfo(oss.str());
	}
}

void ElevatorSystem::assign_passengers_to_elevators() {
	// Временная структура для группировки вызовов по этажам
	std::unordered_map<int, std::vector<int>> floor_requests;

	// Сбор уникальных запросов по этажам
	for (const auto& passenger : waiting_passengers_) {
		floor_requests[passenger.from_floor].push_back(passenger.id);
	}

	// Распределение уникальных этажей
	for (const auto& [floor, passenger_ids] : floor_requests) {
		size_t best_elevator_index = 0;
		int min_distance = std::abs(elevators_[0]->getCurrentFloor() - floor);

		for (size_t i = 1; i < elevators_.size(); ++i) {
			int distance = std::abs(elevators_[i]->getCurrentFloor() - floor);
			if (distance < min_distance) {
				min_distance = distance;
				best_elevator_index = i;
			}
		}
		elevators_[best_elevator_index]->addTargetFloor(floor);
	}
}

void ElevatorSystem::update_elevators() {
	for (auto& elevator : elevators_) {
		elevator->update(waiting_passengers_, current_tick_, waiting_times_, logger_.get());
	}
}

void ElevatorSystem::log_system_state() const {
	for (const auto& elevator : elevators_) {
		std::ostringstream oss;
		oss << "[TICK " << current_tick_ << "] Elevator " << elevator->getId() << " at floor " << elevator->
			getCurrentFloor()
			<< ", state: ";
		switch (elevator->getState()) {
			case ElevatorState::IDLE:
				oss << "IDLE";
				break;
			case ElevatorState::MOVING:
				oss << "MOVING";
				break;
			case ElevatorState::DOORS_OPENING:
				oss << "DOORS_OPENING";
				break;
			case ElevatorState::LOADING:
				oss << "LOADING";
				break;
			case ElevatorState::DOORS_CLOSING:
				oss << "DOORS_CLOSING";
				break;
		}
		oss << ", passengers: " << elevator->getTotalPassengers();
		logger_->LogInfo(oss.str());
	}
}

void ElevatorSystem::print_statistics() {
	int total_passengers = 0;
	int total_floors = 0;

	for (const auto& elevator : elevators_) {
		int id = elevator->getId();
		int passengers = elevator->getTotalPassengers();
		int floors_moved = elevator->getTotalFloorsMoved();
		int capacity = elevator->getCapacity();

		total_passengers += passengers;
		total_floors += floors_moved;

		std::cout << "Elevator " << id << ": "
			<< "Capacity: " << capacity << ", "
			<< "Total passengers transported: " << passengers << ", "
			<< "Floors moved: " << floors_moved << std::endl;
	}

	std::cout << "Total passengers transported: " << total_passengers << std::endl;
	std::cout << "Total floors moved: " << total_floors << std::endl;

	if (!waiting_times_.empty()) {
		double average_wait = std::accumulate(waiting_times_.begin(), waiting_times_.end(), 0.0) /
		                      static_cast<double>(waiting_times_.size());
		std::cout << "Average wait time: " << std::fixed << std::setprecision(2) << average_wait << " ticks" <<
			std::endl;
	} else {
		std::cout << "No passengers were transported." << std::endl;
	}
}