#include "solution.h"


int get_options(int argc, char* argv[], int* N) {
    if (argc != 2) {
        return Invalid_options;
    }
    *N = atoi(argv[1]);
    if (*N <= 0) {
        return Invalid_options;
    }
    return 0;
}

int room_init(RoomState* state, int max_people) {
    if (pthread_mutex_init(&state->lock, NULL)) {
        return Mutex_error;
    }
    if (pthread_cond_init(&state->cond, NULL)) {
        pthread_mutex_destroy(&state->lock);
        return Cond_error;
    }
    state->women_inside = 0;
    state->men_inside = 0;
    state->max_people = max_people;
    state->total_inside = 0;
    return 0;
}

void room_destroy(RoomState* state) {
    pthread_mutex_destroy(&state->lock);
    pthread_cond_destroy(&state->cond);
}


void woman_wants_to_enter(RoomState* state) {
    pthread_mutex_lock(&state->lock);
    while (state->men_inside > 0 || (state->total_inside + 1) > state->max_people) {
        pthread_cond_wait(&state->cond, &state->lock);
    }
    state->women_inside++;
    state->total_inside++;
    pthread_mutex_unlock(&state->lock);
}

void man_wants_to_enter(RoomState* state) {
    pthread_mutex_lock(&state->lock);
    while (state->women_inside > 0 || (state->total_inside + 1) > state->max_people) {
        pthread_cond_wait(&state->cond, &state->lock);
    }
    state->men_inside++;
    state->total_inside++;
    pthread_mutex_unlock(&state->lock);
}


void woman_leaves(RoomState* state) {
    pthread_mutex_lock(&state->lock);
    if (state->women_inside > 0) {
        state->women_inside--;
        state->total_inside--;
    }
    pthread_cond_broadcast(&state->cond);
    pthread_mutex_unlock(&state->lock);
}

void man_leaves(RoomState* state) {
    pthread_mutex_lock(&state->lock);
    if (state->men_inside > 0) {
        state->men_inside--;
        state->total_inside--;
    }
    pthread_cond_broadcast(&state->cond);
    pthread_mutex_unlock(&state->lock);
}

void* woman_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    woman_wants_to_enter(args->state);

    pthread_mutex_lock(args->print_mutex);
    printf("Woman %d enter. Women: %d, Men: %d\n",
           args->id, args->state->women_inside, args->state->men_inside);
    pthread_mutex_unlock(args->print_mutex);

    sleep(1);
    woman_leaves(args->state);

    pthread_mutex_lock(args->print_mutex);
    printf("Woman %d exit. Women: %d, Men: %d\n",
           args->id, args->state->women_inside, args->state->men_inside);
    pthread_mutex_unlock(args->print_mutex);

    free(arg);
    return NULL;
}

void* man_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    man_wants_to_enter(args->state);

    pthread_mutex_lock(args->print_mutex);
    printf("Man %d enter. Women: %d, Men: %d\n",
           args->id, args->state->women_inside, args->state->men_inside);
    pthread_mutex_unlock(args->print_mutex);

    sleep(1);
    man_leaves(args->state);

    pthread_mutex_lock(args->print_mutex);
    printf("Man %d exit. Women: %d, Men: %d\n",
           args->id, args->state->women_inside, args->state->men_inside);
    pthread_mutex_unlock(args->print_mutex);

    free(arg);
    return NULL;
}

int create_thread(pthread_t* thread, void* (*start_routine)(void*), ThreadArgs* args) {
    int err = pthread_create(thread, NULL, start_routine, args);
    if (err != 0) {
        free(args);
        printf("Error creating Thread");
        return Thread_error;
    }
    return 0;
}
