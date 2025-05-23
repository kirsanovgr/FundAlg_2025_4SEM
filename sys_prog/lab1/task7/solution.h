

#ifndef SYS_PROG_SOLUTION_H
#define SYS_PROG_SOLUTION_H

#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIX_MONTHS_IN_SECONDS 15778476

typedef enum {
    Memory_leak = 2,
    Lstat_error,
    Null_pointer_error,
    Open_directory_error
} Errors;

typedef struct {
    char* name;
    struct stat stat;
    char* link_target;
} FileInfo;

typedef struct {
    FileInfo* data;
    size_t capacity;
    size_t size;
} FileInfos;

int init_file_infos(FileInfos* files, size_t capacity, size_t size);

void destroy_files(FileInfos* files);

int insert_file_info(FileInfos* files, const char* name, const struct stat* stat, const char* link_target);

int compare_files(const void* a, const void* b);

int format_permissions(mode_t mode, char *perms_str);

void format_time(time_t mtime, char *time_str);

void ls(const FileInfos* files, const char* path, int is_dir);

int process_file(const char* path);

int process_directory(const char* path, int show_header);

#endif //SYS_PROG_SOLUTION_H
