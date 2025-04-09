#include <stdio.h>
#include <mqueue.h>

mqd_t mq;

typedef enum status {
    OK,
    REALPATH_ERROR,
    ALLOC_ERROR,
    OPENDIR_ERROR,
    MKFIFO_ERROR,
    OPEN_FIFO_ERROR,
    FILENAME_ERROR,
    SERVER_ERROR,
    MQ_CREATE_ERROR
} statuses;

typedef struct riv {
    int river_side;
    // 0 - wolf
    // 1 - goat
    // 2 - cabbage
    char left_side[3][8];
    char right_side[3][8];

} river;