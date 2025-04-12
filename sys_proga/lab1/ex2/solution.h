#ifndef SYS_PROGA_1_2
#define SYS_PROGA_1_2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <linux/limits.h>
#include <math.h>

#define MAX_MASK_LEN 9
#define MAX_STRING_LEN 256

#define BLOCK_SIZE 

typedef enum Errors {
    Wrong_input = 2,
    Memory_leak,
    Invalid_input,
    File_error,
    XORN_error,
    Mask_error,
    Copy_error,
    Find_error,
    Fork_error,
    Wait_error,
    Null_pointer_error,
    Unrecognized_command
} Errors;

// Прототипы функций
int xor_files(char** files, int n_files, int n);
int count_with_mask(char **filenames, int file_count, const char *mask_hex);
int create_copies(char **filenames, int file_count, int n);
int find_string(char **filenames, int file_count, const char *search_str);
int copy_file(const char *filename, int copy_num);
int search_in_file(const char *filename, const char *search_str);
void print_usage();

#endif
