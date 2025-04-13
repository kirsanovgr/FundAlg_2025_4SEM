#ifndef MESSAGES_H
#define MESSAGES_H

#define MAX_USERS 10
#define MSG_SIZE 256
#define STOP_PASSWORD "secret123"

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

#endif
