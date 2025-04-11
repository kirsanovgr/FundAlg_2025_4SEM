#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

mqd_t mq;
mqd_t my_mq;

typedef enum status {
    OK,
    REALPATH_ERROR,
    ALLOC_ERROR,
    OPENDIR_ERROR,
    MKFIFO_ERROR,
    OPEN_FIFO_ERROR,
    FILENAME_ERROR,
    SERVER_ERROR,
    MQ_CREATE_ERROR,
    CONNECTION_ERROR,
    OPEN_PROBLEM
} statuses;

struct mq_attr attr = {
    .mq_flags = 0,
    .mq_maxmsg = 10, 
    .mq_msgsize = 128,   
    .mq_curmsgs = 0
};