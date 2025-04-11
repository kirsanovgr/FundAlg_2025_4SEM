#include "solution.h"

void clear_buffer() {
    char c;
    while ((c = fgetc(stdin)) != EOF && c != '\n');
}

int read_line(char **result) {
    int buffer_size = 16;
    int length = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        return Memory_leak;
    }

    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
        if (length + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);

            if (!new_buffer) {
                free(buffer);
                return Memory_leak;
            }

            buffer = new_buffer;
        }

        buffer[length++] = ch;
    }

    buffer[length] = '\0';

    *result = buffer;

    if (ch != '\n' && ch != EOF) {
        clear_buffer();
    }

    return 0;
}


int check_login(const char* login) {
    if (!login) return 0;
    for (int i = 0; login[i]; ++i) {
        if (i >= MAX_LEN || !isalnum(login[i])) return 0;
    }
    return 1;
}

int check_pin(const int* pin) {
    return (0 <= *pin) && (*pin <= 100000);
}

int init_users(Users** users, size_t capacity, size_t size) {
    (*users) = (Users*)(malloc(sizeof(Users)));
    if (!*users) {
        return Memory_leak;
    }
    (*users)->data = (User*)(malloc(sizeof(User) * capacity));
    if (!(*users)->data) {
        free(*users);
        return Memory_leak;
    }
    (*users)->capacity = capacity;
    (*users)->size = size;
    return 0;
}

int sign_up(Users* users, int* login_index) {
    User user;
    user.login = (char*)malloc(sizeof(char ) * (MAX_LEN + 1));
    if (!user.login) {
        users_destroy(users);
        return Memory_leak;
    }
    printf("Enter a login: ");
    char* login = NULL;
    int tmp = read_line(&login);
    if (tmp)
        return tmp;
    if (!check_login(login)) {
        printf("Invalid login\n");
        return Wrong_input;
    }
    if (users->size != 0) {
    for (size_t i = 0; i < users->size; ++i) {
        if (strcmp(users->data[i].login, login) == 0) {
            printf("This login is already in use\n");
            free(login);
            return Wrong_input;
        }
    }
}
    strncpy(user.login, login, strlen(login));
    user.login[strlen(login)] = '\0';
    free(login);
    printf("Enter a pin: ");
    int pin;
    scanf("%d", &pin);
    clear_buffer();
    if (!check_pin(&pin)) {
        printf("Invalid pin\n");
        return Wrong_input;
    }
    user.pin = pin;
    user.sanctions = -1;
    if (users->capacity == users->size) {
        User* user_tmp = (User*)realloc(users->data, users->capacity * 2 * sizeof(User));
        if (!user_tmp) {
            users_destroy(users);
            return Memory_leak;
        }
        users->capacity *= 2;
        users->data = user_tmp;
    }
    users->data[users->size] = user;
    *login_index = users->size;
    ++users->size;
    return 0;
}

int sign_in(Users* users, int* login_index) {
    printf("Enter a login: ");
    char* login = NULL;
    int tmp = read_line(&login);
    if (tmp)
        return tmp;
    if (!check_login(login)) {
        free(login);
        printf("Invalid login\n");
        return Wrong_input;
    }
    printf("Enter a pin: ");
    int pin;
    scanf("%d", &pin);
    clear_buffer();
    if (!check_pin(&pin)) {
    printf("Invalid pin\n");
        return Wrong_input;
    }
    for (size_t i = 0; i < users->size; ++i) {
        if (strcmp(users->data[i].login, login) == 0 && users->data[i].pin == pin) {
            *login_index = i;
            free(login);
            return 0;
        }
    }
    free(login);
    printf("User does not exit\n");
    return Wrong_input;
}

int get_time(char* buffer) {
    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    strftime(buffer, 9, "%H:%M:%S", time_info);
    return 0;
}

int get_date(char* buffer) {
    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    strftime(buffer, 11, "%d:%m:%Y", time_info);
    return 0;
}

int howmuch_time(const char* date, const char* flag, double* diff) {
    struct tm user_date ={0};

    if (sscanf(date, "%d:%d:%d", &user_date.tm_mday, &user_date.tm_mon, &user_date.tm_year) != 3 ||
        (user_date.tm_mon > 12 || user_date.tm_mon < 0) || user_date.tm_year < 0 || user_date.tm_mday < 0 || user_date.tm_mday > 30) {
        return Wrong_input;
    }
    clear_buffer();

    user_date.tm_mon -= 1;
    user_date.tm_year -= 1900;

    time_t user_time = mktime(&user_date);
    if (user_time == -1) {
        return Wrong_input;
    }

    time_t current_time;
    time(&current_time);

    double diff_seconds = difftime(current_time, user_time);
    if (strcmp(flag, "-s") == 0) {
        *diff = diff_seconds;
    } else if (strcmp(flag, "-m") == 0) {
        *diff = diff_seconds / 60;
    } else if (strcmp(flag, "-h") == 0) {
        *diff = diff_seconds / 3600;
    } else if (strcmp(flag, "-y") == 0) {
        *diff = diff_seconds / 31536000.0;
    } else {
        return Wrong_input;
    }
    return 0;
}

int sacntions(Users* users, char* login, int number) {
    int password;
    printf("ENTER A SPECIAL CODE\n");
    scanf("%d", &password);
    clear_buffer();
    if (password == SECRET_PASS) {
        for (size_t i = 0; i < users->size; ++i) {
            if (strcmp(users->data[i].login, login) == 0) {
                users->data[i].sanctions = number;
                return 0;
            }
        }
        return Wrong_input;
    }
    return Invalid_code;
}


void users_destroy(Users* users) {
    if (!users) return;
    for (size_t i = 0; i < users->size; ++i) {
        free(users->data[i].login);
    }
    free(users->data);
    free(users);
}

int load(const char* filename, Users* users) {
    if (!filename || !users) return Null_pointer_error;
    FILE* fin = fopen(filename, "rb");
    if (!fin) return File_error;

    while (1) {
        size_t login_len;
        if (fread(&login_len, sizeof(size_t), 1, fin) != 1) {
            if (feof(fin)) break;
            fclose(fin);
            return File_error;
        }

        char* login = malloc(login_len);
        if (!login) {
            fclose(fin);
            return Memory_leak;
        }
        if (fread(login, sizeof(char), login_len, fin) != login_len) {
            free(login);
            fclose(fin);
            return File_error;
        }

        User user;
        user.login = login;
        if (fread(&user.pin, sizeof(int), 1, fin) != 1 ||
            fread(&user.sanctions, sizeof(int), 1, fin) != 1) {
            free(login);
            fclose(fin);
            return File_error;
        }

        if (users->size >= users->capacity) {
            users->capacity *= 2;
            User* tmp = realloc(users->data, users->capacity * sizeof(User));
            if (!tmp) {
                free(login);
                fclose(fin);
                return Memory_leak;
            }
            users->data = tmp;
        }
        users->data[users->size++] = user;
    }
    fclose(fin);
    return 0;
}

int save(const char* filename, Users* users) {
    if (!filename || !users) return Null_pointer_error;
    FILE* fin = fopen(filename, "wb");
    if (!fin) return File_error;

    for (size_t i = 0; i < users->size; ++i) {
        User *user = &users->data[i];
        size_t login_len = strlen(user->login) + 1;
        if (fwrite(&login_len, sizeof(size_t), 1, fin) != 1 ||
            fwrite(user->login, sizeof(char), login_len, fin) != login_len ||
            fwrite(&user->pin, sizeof(int), 1, fin) != 1 ||
            fwrite(&user->sanctions, sizeof(int), 1, fin) != 1) {
            fclose(fin);
            return File_error;
        }
    }
    fclose(fin);
    return 0;
}
