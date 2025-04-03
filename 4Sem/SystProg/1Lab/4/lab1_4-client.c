#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAX_MSG_SIZE 1024

typedef enum status {
    OK,
    INCORRECT_NUMBER_OF_ARGUMENTS,
    FILE_ERROR,
    KEY_ERROR,
    QUEUE_ERROR
} status;

typedef struct ClientMessage {
    long mtype;
    int client_id;
    char command[MAX_MSG_SIZE];
} ClientMessage;

typedef struct ServerResponse {
    long mtype;
    char response[MAX_MSG_SIZE];
} ServerResponse;

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Введено некорректное число аргументов командной строки\n");
        return INCORRECT_NUMBER_OF_ARGUMENTS;
    }
    
    key_t key = ftok("server", 'A');
    if (key == -1) {
        printf("Ошибка получения ключа\n");
        return KEY_ERROR;
    }

    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        printf("Ошибка создания очереди сообщений\n");
        return QUEUE_ERROR;
    }
    
    srand(time(NULL) ^ getpid());
    int client_id = rand() % 1000 + 1;
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Ошибка открытия файла\n");
        return FILE_ERROR;
    }
    
    char line[MAX_MSG_SIZE];
    while (fgets(line, sizeof(line), file)) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        ClientMessage msg;
        msg.mtype = 1;
        msg.client_id = client_id;
        strncpy(msg.command, line, MAX_MSG_SIZE);
        
        if (msgsnd(msgid, &msg, sizeof(ClientMessage) - sizeof(long), 0) == -1) {
            printf("Ошибка отправки ответа серверу\n");
            return QUEUE_ERROR;
        }
        
        ServerResponse response;
        if (msgrcv(msgid, &response, sizeof(ServerResponse) - sizeof(long), client_id, 0) == -1) {
            printf("Ошибка получения ответа от сервера\n");
            return QUEUE_ERROR;
        }
        
        printf("Команда: %s\nОтвет: %s\n\n", line, response.response);

        if (strstr(response.response, "Вы проиграли игру!")) {
            break;
        }
    }
    
    fclose(file);
    return 0;
}