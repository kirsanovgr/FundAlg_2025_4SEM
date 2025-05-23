#include "solution.h"

int main(int argc, char* argv[]) {
    int N;
    int err = get_options(argc, argv, &N);
    if (err) {
        printf("Invalid options\n");
        return err;
    }

    pthread_mutex_t print_mutex;
    if (pthread_mutex_init(&print_mutex, NULL) != 0) {
        printf("Mutex init failed\n");
        return Mutex_error;
    }

    RoomState state;
    err = room_init(&state, N);
    switch (err) {
        case Mutex_error:
            pthread_mutex_destroy(&print_mutex);
            printf("Mutex error\n");
            return err;
        case Cond_error:
            pthread_mutex_destroy(&print_mutex);
            printf("Cond error\n");
            return err;
    }

    pthread_t threads[10];
    int thread_count = 0;

    for (int i = 0; i < 5; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->state = &state;
        args->id = i + 1;
        args->print_mutex = &print_mutex;

        if (create_thread(&threads[thread_count], woman_thread, args) == 0) {
            thread_count++;
        } else {
            free(args);
        }

        args = malloc(sizeof(ThreadArgs));
        args->state = &state;
        args->id = i + 6;
        args->print_mutex = &print_mutex;

        if (create_thread(&threads[thread_count], man_thread, args) == 0) {
            thread_count++;
        } else {
            free(args);
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    room_destroy(&state);
    pthread_mutex_destroy(&print_mutex);
    return 0;
}
