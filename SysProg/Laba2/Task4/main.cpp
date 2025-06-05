#include "elevator_system.hpp"

int get_validated_int(const std::string& prompt, int min, int max) {
	int value;
	while (true) {
		std::cout << prompt;
		std::string input;
		std::getline(std::cin, input);
		std::istringstream iss(input);
		if (iss >> value && value >= min && value <= max) {
			return value;
		}
		std::cerr << "Invalid input. Please enter an integer between " << min << " and " << max << "." << std::endl;
	}
}

int main() {
	try {
		int min_ticks = get_validated_int("Enter minimum number of ticks: ", 1, 1000);
		int max_ticks = get_validated_int("Enter maximum number of ticks: ", min_ticks, 1000);
		int floor_count = get_validated_int("Enter number of floors (>=2): ", 2, 100);
		int elevator_count = get_validated_int("Enter number of elevators (>=1): ", 1, 100);
		int elevator_capacity_min = 2;
		int elevator_capacity_max = 8;

		ElevatorSystem system(floor_count, elevator_count, elevator_capacity_min, elevator_capacity_max, min_ticks,
		                      max_ticks);
		system.run();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}