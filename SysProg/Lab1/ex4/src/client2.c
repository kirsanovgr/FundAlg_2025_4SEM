#include "../include/client.h"

statuses connectToServer() {
    mq = mq_open("/server", O_RDWR, 0644, NULL);
    if (mq == -1) {
        return MQ_CREATE_ERROR;
    }
    mq_unlink("/client2");
    my_mq = mq_open("/client2", O_CREAT | O_RDWR, 0644, &attr);
    if (mq == -1) {
        return MQ_CREATE_ERROR;
    }
    if (mq_send(mq, "connect /client2", sizeof("connect /client2"), 0) == -1) {
        return CONNECTION_ERROR;
    }
    char msg[128];
    printf("client2: connect...\n");
    if (mq_receive(my_mq, msg, sizeof(msg), 0) == -1) {
        return CONNECTION_ERROR;
    }
    else {
        printf("client2: %s\n", msg);
    }
    return OK;
    
}

statuses Task() {
    switch(connectToServer()) {
        case MQ_CREATE_ERROR:
            return MQ_CREATE_ERROR;
        case CONNECTION_ERROR:
            return CONNECTION_ERROR;
    }
    char buff[64];
    char buff_to_send[128];
    FILE* in = fopen("../lose.txt", "r");
    if (in == NULL) {
        return OPEN_PROBLEM;
    }
    while (fgets(buff, sizeof(buff), in) != NULL) {
        buff[strcspn(buff, "\n")] = '\0';
        snprintf(buff_to_send, sizeof(buff_to_send), "%s /client2", buff);
        mq_send(mq, buff_to_send, sizeof(buff_to_send), 0);
        mq_receive(my_mq, buff_to_send, sizeof(buff_to_send), 0);
        printf("client2: %s\n", buff_to_send);
        if (strstr(buff_to_send, "YOU LOSE") != NULL || strstr(buff_to_send, "YOU WIN") != NULL) {
            break;
        }
    }
    fclose(in);
    mq_unlink("/client2");
    return OK;

}


int main() {
    switch(Task()) {
        case MQ_CREATE_ERROR:
            printf("mq_create_error\n");
            return MQ_CREATE_ERROR;
        case CONNECTION_ERROR:
            printf("connection error\n");
            return CONNECTION_ERROR;
        case OPEN_PROBLEM:
            printf("open problem\n");
            return OPEN_PROBLEM;
    }
    return OK;

}