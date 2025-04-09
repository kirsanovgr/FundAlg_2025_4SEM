#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

typedef enum status {
    OK,
    REALPATH_ERROR,
    ALLOC_ERROR,
    OPENDIR_ERROR
} statuses;

// у [] приориет выше, чем у *, поэтому в скобках
// иначе получится статический массив указатель на char
typedef struct files_mass {
    char (*mass)[FILENAME_MAX];
    int size;
    int capacity;
} file_mass;

statuses fileMassInit(file_mass** files);
statuses add(file_mass* files, char* file);
void* checkDir(void* arg);