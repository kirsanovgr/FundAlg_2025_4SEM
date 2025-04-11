#ifndef SYS_PROG_SOLUTION_H
#define SYS_PROG_SOLUTION_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

static inline int max(int a, int b) { return (a > b) ? a : b; }
static inline int min(int a, int b) { return (a < b) ? a : b; }

#define N 5
#define KEY 0x1234

typedef enum Errors {
    Sem_error = 2,
    Pthread_error
} Errors;

typedef union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
} semun;

typedef struct ThreadArgs {
    int id;
    int semid;
} ThreadArgs;

void* process(void* arg);

#endif //SYS_PROG_SOLUTION_H
