#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>


typedef enum status {
    OK,
    INCORRECT_NUMBER_OF_ARGUMENTS,
    READLINK_ERROR,
    SHMGET_ERROR,
    SHMAT_ERROR,
    SHMDT_ERROR,
    MEMORY_ERROR,
    DATA_ERROR,
    SEMAPHORE_ERROR
} status;


int main(int argc, char const *argv[]) {
    if (argc != 1) {
        printf("Введено некорректное количество аргументов командной строки\n");
        return INCORRECT_NUMBER_OF_ARGUMENTS; 
    }

    sem_t *client_sem = sem_open("/client_sem", 0);
    sem_t *server_sem = sem_open("/server_sem", 0);
    if (client_sem == SEM_FAILED || server_sem == SEM_FAILED) {
        printf("Ошибка открытия семафоров\n");
        return SEMAPHORE_ERROR;
    }

    char path[PATH_MAX];
    int len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len == -1) {
        printf("Ошибка чтения пути к файлу\n");
        sem_close(client_sem);
        sem_close(server_sem);
        return READLINK_ERROR;
    }   
    path[strlen(path) - 7] = '\0'; 

    key_t key = ftok(path, 'A');
    int shmid = shmget(key, 2048, 0666);
    if (shmid == -1) {
        printf("Ошибка инициализации разделяемой памяти\n");
        sem_close(client_sem);
        sem_close(server_sem);
        return SHMGET_ERROR;
    }

    char *shm = shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        printf("Ошибка подключения разделяемой памяти к клиенту\n");
        sem_close(client_sem);
        sem_close(server_sem);
        return SHMAT_ERROR;
    }
    strcpy(shm, "");
    
    printf("Введите пути к файлам:\nДля завершения введите END\n");
    char pathes[2048] = {0};
    char read_path[PATH_MAX] = {0};
    while (1) {
        fgets(read_path, sizeof(read_path), stdin);
        if (strcmp(read_path, "END\n") == 0) {
            break;
        }
        if (read_path[strlen(read_path) - 1] != '\n') {
            printf("Ошибка чтения пути файла\n");
            shmdt(shm);
            sem_close(client_sem);
            sem_close(server_sem);
            return DATA_ERROR;
        }
        if (strlen(pathes) + strlen(read_path) >= 2048) {
            printf("Введено слишком много путей\n");
            shmdt(shm);
            sem_close(client_sem);
            sem_close(server_sem);
            return DATA_ERROR;
        }

        strcat(pathes, read_path);
    }

    strcpy(shm, pathes);
    printf("Ожидание ответа от сервера...\n");
    sem_post(client_sem);

    sem_wait(server_sem);
    printf("Ответ от сервера:\n%s", shm);
    if (strlen(shm) == 0) {
        printf("Пусто\n");
    }
    
    if (shmdt(shm) == -1) {
        printf("Ошибка отключения разделяемой памяти от клиента\n");
        sem_close(client_sem);
        sem_close(server_sem);
        return SHMDT_ERROR;
    }

    return 0;
}