#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <termios.h>


typedef enum status {
    OK,
    INCORRECT_NUMBER_OF_ARGUMENTS,
    SHMGET_ERROR,
    SHMAT_ERROR,
    READLINK_ERROR,
    SHMDT_ERROR,
    MEMORY_ERROR,
    SEMAPHORE_ERROR
} status;


typedef struct DirEntry {
    char dir[PATH_MAX];
    char files[256][PATH_MAX];
    int file_count;
} DirEntry;


// bool is_directory_exists(const char *path) {
//     struct stat statbuf;
//     if (stat(path, &statbuf)) {
//         return false;
//     }
//     return S_ISDIR(statbuf.st_mode);
// }


// bool is_file_exists(const char *dir_path, const char *file_name) {
//     char full_path[PATH_MAX];
//     snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, file_name);
    
//     struct stat statbuf;
//     if (stat(full_path, &statbuf)) {
//         return false;
//     }
//     return S_ISREG(statbuf.st_mode);
// }


status parse_paths(char *input, DirEntry **entries, int *entry_count, int buf_size) {
    char *input_copy = strdup(input);
    if (input_copy == NULL) {
        return MEMORY_ERROR;
    }

    char *line = strtok(input_copy, "\n");
    while (line != NULL) {
        char dir_path[PATH_MAX];
        char file_name[PATH_MAX];
        strcpy(dir_path, line);
        strcpy(file_name, line);

        char *dir = dirname(dir_path);
        char *file = basename(file_name);

        // if (!is_directory_exists(dir)) {
        //     line = strtok(NULL, "\n");
        //     continue;
        // }

        // if (!is_file_exists(dir, file)) {
        //     line = strtok(NULL, "\n");
        //     continue;
        // }

        bool dir_found = false;
        for (int i = 0; i < *entry_count; i++) {
            if (strcmp((*entries)[i].dir, dir) == 0) {
                strcpy((*entries)[i].files[(*entries)[i].file_count], file);
                (*entries)[i].file_count++;
                dir_found = true;
                break;
            }
        }

        if (!dir_found) {
            strcpy((*entries)[*entry_count].dir, dir);
            strcpy((*entries)[*entry_count].files[0], file);
            (*entries)[*entry_count].file_count = 1;
            (*entry_count)++;
        }

        line = strtok(NULL, "\n");
        if (*entry_count == buf_size) {
            DirEntry *buffer = (DirEntry *)realloc(*entries, sizeof(DirEntry) * *entry_count * 2);
            if (buffer == NULL) {
                return MEMORY_ERROR;
            }
            *entries = buffer;
            buf_size = *entry_count * 2;
        }
    }

    free(input_copy);
    return OK;
}


int has_input() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


int main(int argc, char const *argv[]) {
    if (argc != 1) {
        printf("Введено неверное количество аргументов командной строки для сервера\n");
        return INCORRECT_NUMBER_OF_ARGUMENTS;
    }

    sem_t *client_sem = sem_open("/client_sem", O_CREAT, 0666, 0);
    sem_t *server_sem = sem_open("/server_sem", O_CREAT, 0666, 0);
    if (client_sem == SEM_FAILED) {
        sem_close(client_sem);
        sem_unlink("/client_sem");
        printf("Ошибка создания семафоров\n");
        return SEMAPHORE_ERROR;
    }
    if (server_sem == SEM_FAILED) {
        sem_close(server_sem);
        sem_unlink("/server_sem");
        printf("Ошибка создания семафоров\n");
        return SEMAPHORE_ERROR;
    }

    char path[PATH_MAX];
    if (readlink("/proc/self/exe", path, sizeof(path) - 1) == -1) {
        printf("Ошибка чтения пути к серверу\n");
        sem_close(client_sem);
        sem_close(server_sem);
        sem_unlink("/client_sem");
        sem_unlink("/server_sem");
        return READLINK_ERROR;
    }
    path[strlen(path) - 7] = '\0';
    key_t key = ftok(path, 'A');

    int shmid = shmget(key, 2048, IPC_CREAT | 0666);
    if (shmid == -1) {
        printf("Ошибка создания сегмента разделяемой памяти\n");
        sem_close(client_sem);
        sem_close(server_sem);
        sem_unlink("/client_sem");
        sem_unlink("/server_sem");
        return SHMGET_ERROR;
    }

    char *shm = shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        printf("Ошибка подключения сегмента разделяемой памяти к серверу\n");
        sem_close(client_sem);
        sem_close(server_sem);
        sem_unlink("/client_sem");
        sem_unlink("/server_sem");
        return SHMAT_ERROR;
    }

    printf("Введите <STOP SERVER> для остановки сервера\n");
    int running = 1;
    while(running) {

        if(has_input()) {
            char cmd[20];
            if (fgets(cmd, sizeof(cmd), stdin)) {
                if (strcmp(cmd, "STOP SERVER\n") == 0) {
                    running = 0;
                    continue;
                }
                if (cmd[strlen(cmd) - 1] != '\n') {
                    while (getchar() != '\n');
                }
            }
        }
        sem_wait(client_sem);

        char response[2048] = {0};
        strcpy(response, "");

        DirEntry *result = (DirEntry *)calloc(BUFSIZ, sizeof(DirEntry));
        if (result == NULL) {
            printf("Ошибка выделения памяти\n");
            shmdt(shm);
            shmctl(shmid, IPC_RMID, NULL);
            sem_close(client_sem);
            sem_close(server_sem);
            sem_unlink("/client_sem");
            sem_unlink("/server_sem");
            return MEMORY_ERROR;
        }
        int count = 0;

        status errors = parse_paths(shm, &result, &count, BUFSIZ);
        if (errors == MEMORY_ERROR) {
            printf("Ошибка перевыделения памяти\n");
            free(result);
            shmdt(shm);
            shmctl(shmid, IPC_RMID, NULL);
            sem_close(client_sem);
            sem_close(server_sem);
            sem_unlink("/client_sem");
            sem_unlink("/server_sem");
            return MEMORY_ERROR;
        }

        for (int i = 0; i < count; i++) {
            strcat(response, "DIR: ");
            strcat(response, result[i].dir);
            strcat(response, "\nFILES: ");
            for (int j = 0; j < result[i].file_count; j++) {
                strcat(response, result[i].files[j]);
                strcat(response, " ");
            }
            strcat(response, "\n");
        }

        strcpy(shm, response);
        free(result);

        sem_post(server_sem);
    }

    printf("Сервер завершил работу\n");
    if (shmdt(shm) == -1) {
        printf("Ошибка отключения разделяемой памяти от клиента\n");
        sem_close(client_sem);
        sem_close(server_sem);
        sem_unlink("/client_sem");
        sem_unlink("/server_sem");
        return SHMDT_ERROR;
    }
    shmctl(shmid, IPC_RMID, NULL);

    sem_close(client_sem);
    sem_close(server_sem);
    sem_unlink("/client_sem");
    sem_unlink("/server_sem");

    return 0;
}