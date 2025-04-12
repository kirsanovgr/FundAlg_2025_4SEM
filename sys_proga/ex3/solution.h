#ifndef SYS_PROGA_1_3_H
#define SYS_PROGA_1_3_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

#define N 5
#define KEY 0x1234

static inline int max(int a, int b){return (a > b) ? a : b; }
static inline int min(int a, int b) {return (a < b) ? a : b; }

typedef enum Errors {
    Sem_error = 2,
    Pthread_error
} Errors;

typedef struct ThreadArgs {
    int id;
    int semid;
} ThreadArgs;


typedef union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
} semun;

void* process(void* arg);

#endif // SYS_PROGA_1_3_H