#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define COUNT 10

typedef enum status {
    OK,
    SEM_INIT_ERROR,
    PTHREAD_CREATE_ERROR

} statuses;

sem_t resourses[COUNT];

void* philospherLife(void* arg) {
    int philospher = *(int*) arg;
    for(int i = 0; i < 10; i++) {
        printf("Философ %d думает\n", philospher + 1);
        // берет меньшую по номеру
        if (philospher < (philospher + 1) % 5) {
            sem_wait(&resourses[philospher]);
            //printf("Философ %d взял вилку %d\n", philospher + 1, philospher + 1);
            sem_wait(&resourses[(philospher + 1) % 5]);
            //printf("Философ %d взял вилку %d\n", philospher + 1, (philospher + 1) % 5 + 1);
        }
        else {
            sem_wait(&resourses[(philospher + 1) % 5]);
            //printf("Философ %d взял вилку %d\n", philospher + 1, (philospher + 1) % 5 + 1);
            sem_wait(&resourses[philospher]);
            //printf("Философ %d взял вилку %d\n", philospher + 1, philospher + 1);
        }
        //sleep(1);
        printf("Философ %d ест\n", philospher + 1);
        // кладет большую по номеру
        if (philospher < (philospher + 1) % 5) {
            //printf("Философ %d положил вилку %d\n", philospher + 1, (philospher + 1) % 5 + 1);
            sem_post(&resourses[(philospher + 1) % 5]);
            //printf("Философ %d положил вилку %d\n", philospher + 1, philospher + 1);
            sem_post(&resourses[philospher]);
        }
        else {
            //printf("Философ %d положил вилку %d\n", philospher + 1, philospher + 1);
            sem_post(&resourses[philospher]);
            //printf("Философ %d положил вилку %d\n", philospher + 1, (philospher + 1) % 5 + 1);
            sem_post(&resourses[(philospher + 1) % 5]);
        }
    }

}

int main() {
    pthread_t threads[COUNT];
    int philosphers[COUNT];
    for (int i = 0; i < COUNT; i++) {
        if (sem_init(&resourses[i], 0, 1) == -1) {
            for (int j = i - 1; j >= 0; j--) {
                sem_destroy(&resourses[j]);
            }
            printf("ошибка sem_init\n");
            return SEM_INIT_ERROR;
        }
    }
    for (int i = 0; i < COUNT; i++) {
        philosphers[i] = i;
        if (pthread_create(&threads[i], NULL, philospherLife, &philosphers[i]) != 0) {
            for (int j = 0; j < COUNT; j++) {
                sem_destroy(&resourses[j]);
            }
            printf("ошибка pthread_create\n");
            return PTHREAD_CREATE_ERROR;
        }
    }
    for (int i = 0; i < COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < COUNT; i++) {
        sem_destroy(&resourses[i]);
    }
    printf("Конец трапезы\n");
    return OK;


}