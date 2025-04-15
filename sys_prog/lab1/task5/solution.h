
#ifndef SYS_PROG_SOLUTION_H
#define SYS_PROG_SOLUTION_H

#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>



typedef enum Errors {
    Invalid_options = 2,
    Mutex_error,
    Cond_error,
    Memory_leak,
    Thread_error
} Errors;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int women_inside;
    int men_inside;
    int max_people;
    int total_inside;
} RoomState;

typedef struct {
    RoomState* state;
    int id;
    pthread_mutex_t* print_mutex;
} ThreadArgs;

int room_init(RoomState* state, int max_people);

void room_destroy(RoomState* state);

int get_options(int argc, char* argv[], int* N);

void woman_wants_to_enter(RoomState* state);

void man_wants_to_enter(RoomState* state);

void woman_leaves(RoomState* state);

void man_leaves(RoomState* state);

void* woman_thread(void* arg);

void* man_thread(void* arg);

int create_thread(pthread_t* thread, void* (*start_routine)(void*), ThreadArgs* args);

#endif //SYS_PROG_SOLUTION_H
