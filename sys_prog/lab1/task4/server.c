#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "server.h"

UserState* get_user_state(ServerState *state, int user_id) {
    for (int i = 0; i < state->user_count; i++) {
        if (state->users[i].user_id == user_id) {
            return &state->users[i];
        }
    }
    if (state->user_count < MAX_USERS) {
        UserState new_user = {
                .user_id = user_id,
                .boat_position = 0,
                .in_boat = "",
                .left = {1, 1, 1},
                .right = {0, 0, 0}
        };
        state->users[state->user_count] = new_user;
        state->user_count++;
        return &state->users[state->user_count-1];
    }
    return NULL;
}


int check_danger(UserState *state) {
    int* shore = state->boat_position ? state->left : state->right;
    if (shore[1] == 1) {
        if (shore[0] == 1) return 1;
        if (shore[2] == 1) return 2;
    }
    return 0;
}


void process_command(ServerState *state, ClientMessage *msg, ServerResponse *srv_msg) {
    UserState *user = get_user_state(state, msg->user_id);
    if (!user) {
        strcpy(srv_msg->response, "Error: maximum users exceeded");
        return;
    }

    if (strcmp(msg->command, "take") == 0) {
        if (strlen(user->in_boat) > 0) {
            strcpy(srv_msg->response, "Error: boat is full");
            return;
        }

        int obj_index = -1;
        if (strcmp(msg->object, "wolf") == 0) obj_index = 0;
        else if (strcmp(msg->object, "goat") == 0) obj_index = 1;
        else if (strcmp(msg->object, "cabbage") == 0) obj_index = 2;

        if (obj_index == -1) {
            strcpy(srv_msg->response, "Error: invalid object");
            return;
        }

        int* shore = user->boat_position ? user->right : user->left;
        if (shore[obj_index] == 0) {
            sprintf(srv_msg->response, "Error: %s not available", msg->object);
            return;
        }

        shore[obj_index] = 0;
        strcpy(user->in_boat, msg->object);
        sprintf(srv_msg->response, "Taken %s", msg->object);
    }
    else if (strcmp(msg->command, "stop") == 0) {
        if (strcmp(msg->object, STOP_PASSWORD) == 0) {
            strcpy(srv_msg->response, "Server shutting down...");
            state->running = 0;
        } else {
            strcpy(srv_msg->response, "Error: invalid password");
        }
    }
    else if (strcmp(msg->command, "put") == 0) {
        if (strlen(user->in_boat) == 0) {
            strcpy(srv_msg->response, "Error: nothing to put");
            return;
        }

        int obj_index = -1;
        if (strcmp(user->in_boat, "wolf") == 0) obj_index = 0;
        else if (strcmp(user->in_boat, "goat") == 0) obj_index = 1;
        else if (strcmp(user->in_boat, "cabbage") == 0) obj_index = 2;

        int* shore = user->boat_position ? user->right : user->left;
        shore[obj_index] = 1;
        user->in_boat[0] = '\0';
        sprintf(srv_msg->response, "Put %s", msg->object);
    }
    else if (strcmp(msg->command, "move") == 0) {
        user->boat_position = !user->boat_position;
        int danger = check_danger(user);
        if (danger == 1) {
            strcpy(srv_msg->response, "Game Over: wolf ate the goat!");
        }
        else if (danger == 2) {
            strcpy(srv_msg->response, "Game Over: goat ate the cabbage!");
        }
        else {
            sprintf(srv_msg->response, "Moved to %s shore",
                    user->boat_position ? "right" : "left");
        }
    }
    else {
        strcpy(srv_msg->response, "Error: invalid command");
    }
}

int main() {
    ServerState state = {
            .user_count = 0,
            .running = 1
    };


    key_t key = ftok("msgqueue", 'A');
    int msgid = msgget(key, 0666 | IPC_CREAT);

    while (state.running) {
        ClientMessage cli_msg;
        if (msgrcv(msgid, &cli_msg, sizeof(cli_msg) - sizeof(long), 1, 0) == -1) {
            if (!state.running) break;
            continue;
        }

        ServerResponse srv_msg;
        srv_msg.mtype = cli_msg.user_id;
        process_command(&state, &cli_msg, &srv_msg);

        msgsnd(msgid, &srv_msg, sizeof(srv_msg) - sizeof(long), 0);

        if (!state.running) break;
    }

    msgctl(msgid, IPC_RMID, NULL);
    printf("Server stopped\n");
    return 0;
}
