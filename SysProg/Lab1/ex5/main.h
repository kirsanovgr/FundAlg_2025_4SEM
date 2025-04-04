#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


typedef struct bath {
    int women;
    int men;
} bathroom;

typedef enum status {
    OK, 
    ARGUMENT_ERROR,
    INIT_ERROR,
    THREAD_CREATE_ERROR
} statuses;