#include "solution.h"

void* process(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int id = args->id;
    int semid = args->semid;
    free(arg);

    for (int i = 0; i < 5; ++i) {
        int left = id;
        int right = (id + 1) % N;
        int first = min(left, right);
        int second = max(left, right);

        struct sembuf ops[2] = {
                {first, -1, 0},
                {second, -1, 0}
        };

        semop(semid, ops, 2);
        printf("Philosopher %d eating (forks %d & %d)\n", id, first, second);
        sleep(rand() % 10);

        struct sembuf release_ops[2] = {
                {first, 1, 0},
                {second, 1, 0}
        };
        semop(semid, release_ops, 2);

        printf("Philosopher %d thinking\n", id);
        sleep(rand() % 10);
    }
    return NULL;
}

