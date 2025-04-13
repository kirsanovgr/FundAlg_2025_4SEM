#ifndef SERVER_H
#define SERVER_H

#include "messages.h"

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
