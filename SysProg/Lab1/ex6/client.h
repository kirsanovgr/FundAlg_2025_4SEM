#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define PATH_MAX 4096


sem_t* sem;

typedef enum status {
    OK,
    REALPATH_ERROR,
    ALLOC_ERROR,
    OPENDIR_ERROR,
    MKFIFO_ERROR,
    OPEN_FIFO_ERROR,
    FILENAME_ERROR,
    SERVER_ERROR
} statuses;


statuses connect(int* server_to_client, int* client_to_server);
statuses task(int server_to_client, int client_to_server);