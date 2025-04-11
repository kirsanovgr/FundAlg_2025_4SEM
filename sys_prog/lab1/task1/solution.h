#ifndef SYS_PROG_SOLUTION_H
#define SYS_PROG_SOLUTION_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LEN 6
#define SECRET_PASS 12345
#define EXIT (-1)

typedef enum Errors {
    Wrong_input = 2,
    Memory_leak,
    Invalid_code,
    File_error,
    Null_pointer_error,
    Unrecognized_command
} Errors;

typedef struct User {
    char* login;
    int pin;
    int sanctions;
} User;

typedef struct Users {
    User* data;
    size_t capacity;
    size_t size;
} Users;

void clear_buffer();

int read_line(char** result);

int load(const char* filename, Users* users);

int save(const char* filename, Users* users);

int init_users(Users** users, size_t capacity, size_t size);

int check_login(const char* login);

int check_pin(const int* pin);

int sign_up(Users* users, int* login_index);

int sign_in(Users* users, int* login_index);

int get_time(char* buffer);

int get_date(char* buffer);

int howmuch_time(const char* date, const char* flag, double* diff);

int sacntions(Users* users, char* login, int number);

void users_destroy(Users* users);

void print_menu();

int handle_choice(char* choice, Users* users, int* flag, int* n_commands, int* login_index);

int dialog_manager();


#endif //SYS_PROG_SOLUTION_H
