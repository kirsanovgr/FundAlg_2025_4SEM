#include "server.h"

statuses mqueueCreate() {
    mq = mq_open("/server", O_CREAT | O_RDWR, 0644);
    if (mq == -1) {
        return MQ_CREATE_ERROR;
    }
}


int main() {

}