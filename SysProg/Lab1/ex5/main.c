#include "main.h"

pthread_mutex_t mutex;
pthread_cond_t cond_women;
pthread_cond_t cond_men;
bathroom room = {0, 0};
int max;

void woman_wants_to_enter() {
    pthread_mutex_lock(&mutex);
    while (room.men > 0 || room.women == max) {
        // попал сюда => разблокировал мьютекс
        pthread_cond_wait(&cond_women, &mutex);
        // вышел => снова захватил мьютекс
    }

    room.women++;

    printf("Женщина вошла %d женщин %d мужчин\n", room.women, room.men);

    pthread_mutex_unlock(&mutex);
}

void man_wants_to_enter() {
    pthread_mutex_lock(&mutex);
    while (room.women > 0 || room.men == max) {
        pthread_cond_wait(&cond_men, &mutex);
    }

    room.men++;

    printf("Мужчина вошел %d женщин %d мужчин\n", room.women, room.men);

    pthread_mutex_unlock(&mutex);
}

void woman_leaves() {
    pthread_mutex_lock(&mutex);

    room.women--;
    printf("Женщина вышла %d женщин %d мужчин\n", room.women, room.men);

    if (room.women == 0) {
        // все покидают ожидание на условной, один захватывает мьютекс, остальные ждут, но уже после условной
        pthread_cond_broadcast(&cond_women);
        pthread_cond_broadcast(&cond_men);
    } else if (room.women > 0) {
        pthread_cond_broadcast(&cond_women);
    }

    pthread_mutex_unlock(&mutex);
}

void man_leaves() {
    pthread_mutex_lock(&mutex);

    room.men--;
    printf("Мужчина вышел %d женщин %d мужчин\n", room.women, room.men);

    if (room.men == 0) {
        pthread_cond_broadcast(&cond_women);
        pthread_cond_broadcast(&cond_men);
    } else if (room.men > 0) {
        pthread_cond_broadcast(&cond_men);
    }

    pthread_mutex_unlock(&mutex);
}

void* woman(void* arg) {
    woman_wants_to_enter();
    woman_leaves();
    return NULL;
}

void* man(void* arg) {
    man_wants_to_enter();
    man_leaves();
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("argument error\n");
        return ARGUMENT_ERROR;
    }
    max = atoi(argv[1]);
    if (max <= 0) {
        printf("argument error\n");
        return ARGUMENT_ERROR;
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("init error\n");
        return INIT_ERROR;
    }
    if (pthread_cond_init(&cond_men, NULL) != 0) {
        pthread_mutex_destroy(&mutex);
        printf("init error\n");
        return INIT_ERROR;
    }
    if (pthread_cond_init(&cond_women, NULL) != 0) {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond_men);
        printf("init error\n");
        return INIT_ERROR;
    }
    pthread_t threads[10];

    for (int i = 0; i < 5; i++) {
        if (pthread_create(&threads[i], NULL, woman, NULL) != 0) {
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&cond_men);
            pthread_cond_destroy(&cond_women);
            printf("thread create error\n");
            return THREAD_CREATE_ERROR;
        }
        if (pthread_create(&threads[i], NULL, man, NULL) != 0) {
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&cond_men);
            pthread_cond_destroy(&cond_women);
            printf("thread create error\n");
            return THREAD_CREATE_ERROR;
        }
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_men);
    pthread_cond_destroy(&cond_women);

    return OK;
}