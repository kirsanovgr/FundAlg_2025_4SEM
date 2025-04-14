#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>

typedef enum status {
    OK,
    OPEN_PROBLEM,
    INCORRECT_ARGUMENTS,
    ALLOC_ERROR,
    FORK_ERROR,
    FIND
} statuses;