#include "server.h"


statuses fileMassInit(file_mass** files) {
    (*files) = (file_mass*) malloc(sizeof(file_mass));
    (*files)->size = 0;
    (*files)->capacity = 10;
    (*files)->mass = (char(*)[FILENAME_MAX]) malloc((*files)->capacity * sizeof(char[FILENAME_MAX]));
    if ((*files)->mass == NULL) {
        return ALLOC_ERROR;
    }
    return OK;
}

statuses add(file_mass* files, char* file) {
    if (files->size + 1 == files->capacity) {
        files->capacity = 2 * files->capacity;
        char (*tmp)[FILENAME_MAX] = (char(*)[FILENAME_MAX]) realloc(files->mass, files->capacity * sizeof(char[FILENAME_MAX]));
        if (tmp == NULL) {
            return ALLOC_ERROR;
        }
        files->mass = tmp;
    }
    strcpy(files->mass[files->size], file);
    files->size++;
    return OK;
}

file_mass* files(char* file) {
    file_mass* files;
    if (fileMassInit(&files) != OK) {
        return NULL;
    }
    char* dir_name = dirname(file);
    DIR* dir = opendir(dir_name);
    struct dirent* current_file;
    while((current_file = readdir(dir)) != NULL) {
        if (strcmp(current_file->d_name, ".") == 0 || 
            strcmp(current_file->d_name, "..") == 0) {
            continue;
        }
        if (add(files, current_file->d_name) != OK) {
            free(files->mass);
            free(files);
            return NULL;
        }

    }
    closedir(dir);
    return files;


}

statuses makeFIFO(int* server_to_client, int* client_to_server) {
    sem = sem_open("/sem", O_CREAT, 0644, 1);
    if (mkfifo("/tmp/serverToClient", 0666) == -1) {
        return MKFIFO_ERROR;
    }
    if (mkfifo("/tmp/clientToServer", 0666) == -1) {
        return MKFIFO_ERROR;
    }
    *server_to_client = open("/tmp/serverToClient", O_WRONLY);
    *client_to_server = open("/tmp/clientToServer", O_RDONLY);
    if (*server_to_client == -1 || *client_to_server == -1) {
        return OPEN_FIFO_ERROR;
    }
    return OK;
}

statuses task(int server_to_client, int client_to_server) {
    char buff[PATH_MAX];
    int count;
    char path[PATH_MAX];
    char numb[10];
    int len = 0;
    while(1) {
        len = 0;
        count = read(client_to_server, buff, sizeof(buff));
        buff[count - 1] = 0;
        if (buff[0] == 'q' && buff[1] == 'u' && buff[2] == 'i' && buff[3] == 't') {
            return OK;
        }
        if (realpath(buff, path) == NULL) {
            write(server_to_client, "!", sizeof("!"));
            continue;
        }
        file_mass* ans = files(path);
        if (ans == NULL) {
            write(server_to_client, "?", sizeof("?"));
            return ALLOC_ERROR;
        }
        for (int i = 0; i < ans->size; i++) {
            len += strlen(ans->mass[i]) + 1;
        }
        snprintf(numb, sizeof(numb), "%d", len);
        sem_wait(sem);
        write(server_to_client, numb, sizeof(numb));
        for (int i = 0; i < ans->size; i++) {
            write(server_to_client, ans->mass[i], strlen(ans->mass[i]));
            write(server_to_client, "\n", 1);
        }
        sem_post(sem);
        free(ans->mass);
        free(ans);

    }
    return OK;

}

void clean() {
    unlink("/tmp/serverToClient");
    unlink("/tmp/clientToServer");
    sem_close(sem);
    sem_unlink("/sem");
}


int main() {
    atexit(clean);
    int server_to_client;
    int client_to_server;
    switch(makeFIFO(&server_to_client, &client_to_server)) {
        case MKFIFO_ERROR:
            printf("mkfifo error\n");
            return MKFIFO_ERROR;
        case OPEN_FIFO_ERROR:
            printf("open fifo error\n");
            return OPEN_FIFO_ERROR;
    }
    if (task(server_to_client, client_to_server) == ALLOC_ERROR) {
        printf("alloc error");
        return ALLOC_ERROR;
    }
    return OK;

    

}