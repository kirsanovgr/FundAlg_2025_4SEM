#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAX_MSG_SIZE 1024
#define MAX_CLIENTS 10

typedef enum status {
    OK,
    INCORRECT_NUMBER_OF_ARGUMENTS,
    GAME_OVER,
    KEY_ERROR,
    QUEUE_ERROR
} status;

typedef enum Object { 
    WOLF, 
    GOAT, 
    CABBAGE, 
    NOTHING 
} Object;

typedef struct {
    long mtype;
    int client_id;
    char command[MAX_MSG_SIZE];
} ClientMessage;

typedef struct {
    long mtype;
    char response[MAX_MSG_SIZE];
} ServerResponse;

typedef struct {
    Object boat;
    Object left_shore[3];
    Object right_shore[3];
    int left_count;
    int right_count;
    int boat_position;
    int client_ids[MAX_CLIENTS];
    int num_clients;
} GameState;

void initialize_game(GameState *state) {
    state->boat = NOTHING;
    state->left_shore[0] = WOLF;
    state->left_shore[1] = GOAT;
    state->left_shore[2] = CABBAGE;
    state->right_shore[0] = NOTHING;
    state->right_shore[1] = NOTHING;
    state->right_shore[2] = NOTHING;
    state->left_count = 3;
    state->right_count = 0;
    state->boat_position = 0;
    state->num_clients = 0;
}

status check_danger(GameState *state) {
    if (state->boat_position == 0) {
        int wolf_left = 0, goat_left = 0, cabbage_left = 0;
        for (int i = 0; i < state->left_count; i++) {
            if (state->left_shore[i] == WOLF) {
                wolf_left = 1;
            }
            if (state->left_shore[i] == GOAT) {
                goat_left = 1;
            }
            if (state->left_shore[i] == CABBAGE) {
                cabbage_left = 1;
            }
        }
        
        if (wolf_left && goat_left && state->boat != GOAT) {
            return GAME_OVER;
        }
        if (goat_left && cabbage_left && state->boat != GOAT) {
            return GAME_OVER;
        }
    } else {
        int wolf_right = 0, goat_right = 0, cabbage_right = 0;
        for (int i = 0; i < state->right_count; i++) {
            if (state->right_shore[i] == WOLF) {
                wolf_right = 1;
            }
            if (state->right_shore[i] == GOAT) {
                goat_right = 1;
            }
            if (state->right_shore[i] == CABBAGE) {
                cabbage_right = 1;
            }
        }
        
        if (wolf_right && goat_right && state->boat != GOAT) {
            return GAME_OVER;
        }
        if (goat_right && cabbage_right && state->boat != GOAT) {
            return GAME_OVER;
        }
    }
    return OK;
}

status process_command(GameState *state, char *command, char *response) {
    if (strncmp(command, "take ", 5) == 0) {
        if (state->boat != NOTHING) {
            strcpy(response, "Ошибка: Лодка уже заполнена\n");
            return GAME_OVER;
        }
        
        Object obj = NOTHING;
        if (strstr(command, "wolf")) {
            obj = WOLF;
        } else if (strstr(command, "goat")) {
            obj = GOAT;
        } else if (strstr(command, "cabbage")) {
            obj = CABBAGE;
        } else {
            strcpy(response, "Ошибка: неизвестный объект\n");
            return GAME_OVER;
        }
        
        int found = 0;
        if (state->boat_position == 0) {
            for (int i = 0; i < state->left_count; i++) {
                if (state->left_shore[i] == obj) {
                    found = 1;
                    for (int j = i; j < state->left_count - 1; j++) {
                        state->left_shore[j] = state->left_shore[j+1];
                    }
                    state->left_count--;
                    break;
                }
            }
        } else {
            for (int i = 0; i < state->right_count; i++) {
                if (state->right_shore[i] == obj) {
                    found = 1;
                    for (int j = i; j < state->right_count - 1; j++) {
                        state->right_shore[j] = state->right_shore[j+1];
                    }
                    state->right_count--;
                    break;
                }
            }
        }
        
        if (!found) {
            strcpy(response, "Ошибка: объект не найден на данном берегу\n");
            return GAME_OVER;
        }
        
        state->boat = obj;
        sprintf(response, "Взят %s", command + 5);
        return OK;
    } else if (strncmp(command, "put", 3) == 0) {
        if (state->boat == NOTHING) {
            strcpy(response, "Ошибка: лодка пустая\n");
            return GAME_OVER;
        }
        
        Object obj = state->boat;
        if (state->boat_position == 0) {
            state->left_shore[state->left_count++] = obj;
        } else {
            state->right_shore[state->right_count++] = obj;
        }
        
        state->boat = NOTHING;
        strcpy(response, "Объект поставлен на берег");
        return OK;
    } else if (strncmp(command, "move", 4) == 0) {
        state->boat_position = !state->boat_position;
        if (check_danger(state)) {
            strcpy(response, "Ошибка: кого-то сожрали после перевозки\n");
            return GAME_OVER;
        }
        sprintf(response, "Перевезен на %s берег", state->boat_position ? "правый" : "левый");
        return OK;
    } else {
        strcpy(response, "Ошибка: неизвестная команда\n");
        return GAME_OVER;
    }
    return OK;
}

int is_game_won(GameState *state) {
    return state->right_count == 3 && state->boat_position == 1;
}

int main(int argc, char const *argv[]) {
    if (argc != 1) {
        printf("Введено некорректное количество аргументов командной строки\n");
        return INCORRECT_NUMBER_OF_ARGUMENTS;
    }
    key_t key = ftok("server", 'A');
    if (key == -1) {
        printf("Ошибка получения ключа\n");
        return KEY_ERROR;
    }
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("Ошибка создания очереди сообщений\n");
        return QUEUE_ERROR;
    }
    
    GameState state;
    initialize_game(&state);
    
    printf("Сервер запущен. Ждем клиента...\n");
    printf("Для остановки сервера передайте команду <STOP SERVER> через очередь сообщений\n");
    
    while (1) {
        ClientMessage msg;
        if (msgrcv(msgid, &msg, sizeof(ClientMessage) - sizeof(long), 1, 0) == -1) {
            printf("Ошибка получения ответа от клиента\n");
            msgctl(msgid, IPC_RMID, NULL);
            return QUEUE_ERROR;
        }
        
        int client_found = 0;
        for (int i = 0; i < state.num_clients; i++) {
            if (state.client_ids[i] == msg.client_id) {
                client_found = 1;
                break;
            }
        }
        
        if (!client_found) {
            if (state.num_clients < MAX_CLIENTS) {
                state.client_ids[state.num_clients++] = msg.client_id;
                printf("Новый клиент подключен: %d\n", msg.client_id);
            } else {
                printf("Достигнуто максимальное количество клиентов. Отклонение %d\n", msg.client_id);
                continue;
            }
        }
        
        printf("Получен ответ от клиента %d: %s\n", msg.client_id, msg.command);

        if (strncmp(msg.command, "STOP SERVER", 11) == 0) {
            break;
        }
        
        ServerResponse response;
        response.mtype = msg.client_id;


        status pos = process_command(&state, msg.command, response.response);
        printf("Текущее состояние: Левый берег: %d объектов, Правый берег: %d объектов, Лодка: %s, Позиция: %s\n",
            state.left_count, state.right_count,
            state.boat == WOLF ? "wolf" : state.boat == GOAT ? "goat" : state.boat == CABBAGE ? "cabbage" : "empty",
            state.boat_position ? "правый берег" : "левый берег");
        
        if (!pos) {
            if (is_game_won(&state)) {
                strcat(response.response, "\n\nВы выиграли игру!");
                initialize_game(&state);
            }
        } else {
            strcat(response.response, "\nВы проиграли игру!");
            initialize_game(&state);
        }
        
        if (msgsnd(msgid, &response, sizeof(ServerResponse) - sizeof(long), 0) == -1) {
            printf("Ошибка отправки сообщения клиенту\n");
            msgctl(msgid, IPC_RMID, NULL);
            return QUEUE_ERROR;
        }
    }
    
    printf("Сервер завершил работу!\n");
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}