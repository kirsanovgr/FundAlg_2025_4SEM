#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <stdlib.h>

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
    char client[100];
    mqd_t client_ds;
    int boat;
    // -1 - пусто
    // 0 - wolf
    // 1 - goat
    // 2 - cabbage

    int river_side;
    // 0 - левый
    // 1 - правый

    int left_side[3];
    int right_side[3];
     // 0 - отсуствует
    // 1 - присутсвует

} river;

struct mq_attr attr = {
    .mq_flags = 0,
    .mq_maxmsg = 10, 
    .mq_msgsize = 128,   
    .mq_curmsgs = 0
};