
#include "array"
#include "include/unility.h"

int main(int argc, char* argv[]) {
    std::pair<size_t, size_t> input = get_args(argc, argv);

    House house(input.first, input.second);
    house.work();


    std::cout << "Statistic:\n1. Number of people transported:\n";
    size_t sum = 0;
    for(const Elevator & elevator: house.elevators) {
        sum += elevator.elevator_statistics.total_number_transported_people;
        std::cout << "\t1: " << elevator.elevator_statistics.total_number_transported_people << std::endl;
    }
    std::cout << "2. Total number of people transported: " << sum << std::endl;
    std::cout << "3. Number of floors covered by each elevator:\n";
    sum = 0;
    for(const Elevator & elevator: house.elevators) {
        sum += elevator.elevator_statistics.total_number_covered_floors;
        std::cout << "\t1: " << elevator.elevator_statistics.total_number_covered_floors << std::endl;
    }
    std::cout << "2. Total number of floors covered: " << sum << std::endl;

    double t = 0;
    for(const Elevator & elevator: house.elevators) {
        t += elevator.elevator_statistics.average_waiting_time;
    }
    std::cout << "Total average waiting time: " << t / (double )house.elevators.size() << std::endl;
    return 0;
}