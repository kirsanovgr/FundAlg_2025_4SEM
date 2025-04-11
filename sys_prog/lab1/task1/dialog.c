#include "solution.h"

void print_menu() {
    printf("1 - Sign In\n");
    printf("2 - Sigh Up\n");
    printf("3 - Time\n");
    printf("4 - Date\n");
    printf("5 - Howmuch <time> flag\n");
    printf("6 - Logout\n");
    printf("7 - Sanctions\n");
    printf("8 - Exit\n");
}

int handle_choice(char* choice, Users* users, int* flag, int* n_commands, int* login_index) {
    if (strcmp(choice, "Exit") == 0) {
        return EXIT;
    }

    int err = 0;
    if (*flag == 0) {
        if (strcmp(choice, "Sign In") == 0) {
            err = sign_in(users, login_index);
            if (err == 0) {
                *flag = 1;
            }
            return err;
        } else if (strcmp(choice, "Sign Up") == 0) {
            err = sign_up(users, login_index);
            if (err == 0) {
                *flag = 1;
            }
            return err;
        }
    }

    if (*flag == 0) {
        printf("You have not been authorized\n");
        return Wrong_input;
    }

    if (strcmp(choice, "Logout") == 0) {
        *flag = 0;
        *n_commands = 0;
        return 0;
    }
    if (*flag == 1 && (*n_commands <= users->data[*login_index].sanctions || users->data[*login_index].sanctions == -1)) {
        if (strcmp(choice, "Time") == 0) {
            char buf[11];
            err = get_time(buf);
            if (err) {
                return err;
            }
            printf("%s\n", buf);
        } else if (strcmp(choice, "Date") == 0) {
            char buf[11];
            err = get_date(buf);
            if (err) {
                return err;
            }
            printf("%s\n", buf);
        } else if (strncmp(choice, "Howmuch", 7) == 0) {
            double diff = 0;
            char time[15];
            char time_flag[3];
            int parsed = sscanf(choice, "Howmuch %14s %2s\n", time, time_flag);
            if (parsed != 2) {
                return Wrong_input;
            }
            err = howmuch_time(time, time_flag, &diff);
            if (err)
                return err;
            printf("%f\n", diff);
        } else if (strncmp(choice, "Sanctions", 9) == 0) {
            char login[7];
            int n_sanctions;
            int parsed = sscanf(choice, "Sanctions %6s %d\n", login, &n_sanctions);
            if (parsed != 2) {
                return Wrong_input;
            }
            err = sacntions(users, login, n_sanctions);
            if (err)
                return err;

        } else {
            printf("Unrecognized command\n");
            return Unrecognized_command;
        }
    } else if (*n_commands > users->data[*login_index].sanctions && *flag == 1) {
        printf("The number of requests exceeds the possible\n");
    }
    return 0;
}

int dialog_manager() {
    Users* users;
    int err = init_users(&users, 2, 0);
    if (err) {
        return err;
    }
    err = load("lab1/task1/database.bin", users);
    if (err) {
        return err;
    }
    int is_authorized = 0,  n_commands = 0, login_index;
    print_menu();
    while (1) {
        char* choice = NULL;
        err = read_line(&choice);
        if (err) {
            users_destroy(users);
            return Memory_leak;
        }
        err = handle_choice(choice, users, &is_authorized, &n_commands, &login_index);
        if (err == EXIT) {
            err = save("lab1/task1/database.bin", users);
            if (err)
                return err;
            users_destroy(users);
            free(choice);
            return 0;
        } else if (err == Memory_leak) {
            free(choice);
            return err;
        } else if (err == Wrong_input) {
            printf("Wrong input\n");
        }
        if (is_authorized && users->data[login_index].sanctions != -1 && strcmp(choice, "Logout") != 0) {
            ++n_commands;
        }
        free(choice);
        print_menu();
    }
}
