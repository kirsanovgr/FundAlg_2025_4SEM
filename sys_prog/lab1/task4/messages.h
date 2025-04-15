#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_USERS 10
#define MSG_SIZE 256
#define STOP_PASSWORD "secret123"

typedef enum Errors {
    INVALID_ARGS = 2,
    FILE_ERROR,
    MSG_ERROR
} Errors;


typedef struct {
    long mtype;
    int user_id;
    char command[10];
    char object[10];
} ClientMessage;

typedef struct {
    long mtype;
    char response[MSG_SIZE];
} ServerResponse;

typedef struct {
    int user_id;
    int boat_position;
    char in_boat[10];
    int left[3];
    int right[3];
} UserState;

typedef struct {
    UserState users[MAX_USERS];
    int user_count;
    int running;
} ServerState;

UserState* get_user_state(ServerState *state, int user_id);
void process_command(ServerState *state, ClientMessage *msg, ServerResponse *srv_msg);
int check_danger(UserState *state);

#endif
