#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <termios.h>
#include "messages.h"


void get_password(char *password, size_t max_len) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Enter shutdown password: ");
    fgets(password, max_len, stdin);
    password[strcspn(password, "\n")] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <command_file>\n", argv[0]);
        exit(1);
    }


    FILE *file = fopen(argv[1], "r");
    if (!file) {
        exit(1);
    }


    key_t key = ftok("msgqueue", 'A');
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        fprintf(stderr, "Check if server is running\n");
        exit(1);
    }

    int user_id = getpid();
    printf("User ID: %d\n", user_id);

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        char cmd[10] = {0}, obj[10] = {0};
        sscanf(line, "%s %s", cmd, obj);


        if (strcmp(cmd, "stop") == 0) {
            char password[20];
            get_password(password, sizeof(password));
            strncpy(obj, password, sizeof(obj)-1);
        }

        ClientMessage cli_msg = {
                .mtype = 1,
                .user_id = user_id
        };
        strncpy(cli_msg.command, cmd, sizeof(cli_msg.command)-1);
        strncpy(cli_msg.object, obj, sizeof(cli_msg.object)-1);

        if (msgsnd(msgid, &cli_msg, sizeof(cli_msg) - sizeof(long), 0) == -1) {
            continue;
        }

        ServerResponse srv_msg;
        if (msgrcv(msgid, &srv_msg, sizeof(srv_msg) - sizeof(long), user_id, 0) == -1) {
            continue;
        }

        printf("Server response: %s\n", srv_msg.response);

        if (strstr(srv_msg.response, "shutting down")) {
            break;
        }
        if (strstr(srv_msg.response, "Game Over")) {
            break;
        }
    }

    fclose(file);
    return 0;
}
