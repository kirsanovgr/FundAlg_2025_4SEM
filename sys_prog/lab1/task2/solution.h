
#ifndef SYS_PROG_SOLUTION_H
#define SYS_PROG_SOLUTION_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

typedef enum {
    OK = 0,
    INVALID_OPTIONS = 2,
    MEMORY_LEAK,
    FILE_ERROR,
    OVERFLOW_ERROR
} Errors;

typedef enum Flags { XOR, MASK, COPY, FIND } Flags;

void* my_memmem(const void* haystack, size_t haystacklen,
                const void* needle, size_t needlelen);

int process_xorN(char** files, int n_files, int N);
int process_mask(char** files, int n_files, uint32_t mask);
int process_copyn(char** files, int n_files, int N);
int process_find(char** files, int n_files, const char* search_str);

#endif //SYS_PROG_SOLUTION_H
