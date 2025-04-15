#include "messages.h"

void get_password(char *password, size_t max_len) {
    printf("Enter shutdown password: ");

    if (fgets(password, max_len, stdin) == NULL) {
        password[0] = '\0';
        return;
    }

    password[strcspn(password, "\n")] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid args\n");
        return INVALID_ARGS;
    }


    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("File Error\n");
        return FILE_ERROR;
    }


    key_t key = ftok("msgqueue", 'A');
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        printf("Message error\n");
        return MSG_ERROR;
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
        sleep(1);
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
