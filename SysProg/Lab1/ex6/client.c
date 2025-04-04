#include "client.h"


statuses connect(int* server_to_client, int* client_to_server) {
    *server_to_client = open("/tmp/serverToClient", O_RDONLY);
    *client_to_server = open("/tmp/clientToServer", O_WRONLY);
    sem = sem_open("/sem", 0);
    if (*server_to_client == -1 || *client_to_server == -1) {
        return OPEN_FIFO_ERROR;
    }
    return OK;

}

statuses task(int server_to_client, int client_to_server) {
    char buff[PATH_MAX];
    int count;
    int ans_size;
    char numb[10];
    char* msg;
    while ((count = read(0, buff, sizeof(buff))) > 0) {
        if (buff[0] == 'q' && buff[1] == 'u' && buff[2] == 'i' && buff[3] == 't') {
            write(client_to_server, buff, sizeof(buff));
            break;
        }
        write(client_to_server, buff, count);
        read(server_to_client, numb, sizeof(numb));
        if (numb[0] == '?') {
            return SERVER_ERROR;
        }
        else if(numb[0] == '!') {
            printf("INCORRECT INPUT!\n");
            continue;
        }
        sem_wait(sem);
        ans_size = atoi(numb);
        ans_size = ans_size / sizeof(buff) + 1;
        printf("\n");
        for(int i = 0; i < ans_size; i++) {
            memset(buff, 0, sizeof(buff));
            read(server_to_client, buff, sizeof(buff));
            msg = strtok(buff, "\n");
            while(msg != NULL) {
                printf("%s\n", msg);
                msg = strtok(NULL, "\n");
            }

        }
        sem_post(sem);
        printf("\n");
    }
    return OK;
}


int main() {
    int server_to_client;
    int client_to_server;
    if (connect(&server_to_client, &client_to_server) == OPEN_FIFO_ERROR) {
        printf("connection error\n");
        return OPEN_FIFO_ERROR;
    }
    if (task(server_to_client, client_to_server) == SERVER_ERROR) {
        printf("server error\n");
        return SERVER_ERROR;
    }
    return OK;


}