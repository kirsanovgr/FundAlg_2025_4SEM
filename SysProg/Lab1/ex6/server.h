#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define PATH_MAX 4096


typedef struct files_mass {
    char (*mass)[FILENAME_MAX];
    int size;
    int capacity;
} file_mass;

typedef enum status {
    OK,
    REALPATH_ERROR,
    ALLOC_ERROR,
    OPENDIR_ERROR,
    MKFIFO_ERROR,
    OPEN_FIFO_ERROR,
    FILENAME_ERROR
} statuses;

sem_t* sem;

statuses fileMassInit(file_mass** files);
statuses add(file_mass* files, char* file);
file_mass* files(char* file);
statuses makeFIFO(int* server_to_client, int* client_to_server);
statuses task(int server_to_client, int client_to_server);