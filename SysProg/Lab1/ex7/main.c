#include "main.h"

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

void* checkDir(void* arg) {
    file_mass* files;
    if (fileMassInit(&files) != OK) {
        return NULL;
    }
    DIR* dir = (DIR*) arg;
    struct dirent* current_file;
    while((current_file = readdir(dir)) != NULL) {
        if (strcmp(current_file->d_name, ".") == 0 || 
            strcmp(current_file->d_name, "..") == 0) {
            continue;
        }
        if (add(files, current_file->d_name) != OK) {
            return NULL;
        }

    }
    return (void*) files;
}


int main(int argc, char** argv) {
    if (argc > 1) {
        DIR* dir[argc - 1];
        pthread_t threads[argc - 1];
        file_mass* ans[argc - 1];
        for (int i = 1; i < argc; i++) {
            dir[i - 1] = opendir(argv[i]);
            if (dir[i - 1] == NULL) {
                printf("opendir error\n");
                return OPENDIR_ERROR;
            }
            pthread_create(&threads[i - 1], NULL, checkDir, (void*) dir[i - 1]);
        }
        for (int i = 1; i < argc; i++) {
            pthread_join(threads[i - 1], (void**) &ans[i - 1]);
            closedir(dir[i - 1]);
            
            if (ans[i - 1] == NULL) {
                return ALLOC_ERROR;
            }
            printf("%s\n", argv[i]);
            for(int j = 0; j < ans[i - 1]->size; j++) {
                printf("    %s\n", ans[i - 1]->mass[j]);
            }
            free(ans[i - 1]->mass);
            free(ans[i - 1]);
        }
    }
    else {
        DIR* dir = opendir(".");
        if (dir == NULL) {
            printf("opendir error\n");
            return OPENDIR_ERROR;
        }
        file_mass* ans = (file_mass*) checkDir((void*) dir);
        printf(".\n");
        for (int i = 0; i < ans->size; i++) {
            printf("    %s\n", ans->mass[i]);
        }
        free(ans->mass);
        free(ans);


    }
    return OK;

}