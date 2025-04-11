#include "solution.h"

int init_file_infos(FileInfos* files, size_t capacity, size_t size) {
    files->data = malloc(sizeof(FileInfo) * capacity);
    if (!files->data) {
        return Memory_leak;
    }
    files->capacity = capacity;
    files->size = size;
    return 0;
}

void destroy_files(FileInfos* files) {
    for (size_t i = 0; i < files->size; i++) {
        free(files->data[i].name);
        free(files->data[i].link_target);
    }
    free(files->data);
}

int compare_files(const void* a, const void* b) {
    const FileInfo* fa = (const FileInfo*)a;
    const FileInfo* fb = (const FileInfo*)b;
    return strcoll(fa->name, fb->name);
}

int insert_file_info(FileInfos* files, const char* name, const struct stat* stat, const char* link_target) {
    if (files->size >= files->capacity) {
        size_t new_capacity = files->capacity * 2;
        FileInfo* new_data = realloc(files->data, sizeof(FileInfo) * new_capacity);
        if (!new_data) {
            return Memory_leak;
        }
        files->data = new_data;
        files->capacity = new_capacity;
    }

    FileInfo* info = &files->data[files->size];

    info->name = strdup(name);
    if (!info->name) {
        destroy_files(files);
        return Memory_leak;
    }

    info->stat = *stat;

    if (link_target) {
        info->link_target = strdup(link_target);
        if (!info->link_target) {
            destroy_files(files);
        }
    } else {
        info->link_target = NULL;
    }

    files->size++;
    return 0;
}



int format_permissions(mode_t mode, char *perms_str) {

    if (!perms_str) {
        return Null_pointer_error;
    }

    perms_str[0] = (S_ISDIR(mode)) ? 'd' :
                   (S_ISLNK(mode)) ? 'l' :
                   (S_ISFIFO(mode)) ? 'p' :
                   (S_ISSOCK(mode)) ? 's' :
                   (S_ISCHR(mode)) ? 'c' :
                   (S_ISBLK(mode)) ? 'b' : '-';

    perms_str[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms_str[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms_str[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms_str[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms_str[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms_str[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms_str[7] = (mode & S_IROTH) ? 'r' : '-';
    perms_str[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms_str[9] = (mode & S_IXOTH) ? 'x' : '-';
    perms_str[10] = '\0';

    if (mode & S_ISUID) perms_str[3] = (perms_str[3] == 'x') ? 's' : 'S';
    if (mode & S_ISGID) perms_str[6] = (perms_str[6] == 'x') ? 's' : 'S';
    if (mode & S_ISVTX) perms_str[9] = (perms_str[9] == 'x') ? 't' : 'T';
    return 0;
}

void format_time(time_t mtime, char *time_str) {
    struct tm* time_info = localtime(&mtime);
    time_t now = time(NULL);

    if (difftime(mtime, now) > SIX_MONTHS_IN_SECONDS) {
        strftime(time_str, 20, "%b %e  %Y", time_info);
    } else {
        strftime(time_str, 20, "%b %e %H:%M", time_info);
    }
}

void ls(const FileInfos* files, const char* path, int is_dir) {
    if (is_dir) {
        printf("%s:\n", path);
    }

    for (size_t i = 0; i < files->size; i++) {
        const FileInfo* info = &files->data[i];
        char perms[11];
        char time_str[20];
        char link_info[1024] = {0};

        format_permissions(info->stat.st_mode, perms);
        format_time(info->stat.st_mtime, time_str);

        if (S_ISLNK(info->stat.st_mode) && info->link_target) {
            snprintf(link_info, sizeof(link_info), " -> %s", info->link_target);
        }

        printf("%s %2ld %-8s %-8s %8ld %s %s%s\n",
               perms,
               info->stat.st_nlink,
               getpwuid(info->stat.st_uid)->pw_name,
               getgrgid(info->stat.st_gid)->gr_name,
               info->stat.st_size,
               time_str,
               info->name,
               link_info);
    }
    if (is_dir && files->size > 0) printf("\n");
}


int process_file(const char* path) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        return Lstat_error;
    }

    FileInfos files;
    init_file_infos(&files, 2, 0);
    insert_file_info(&files, path, &st, NULL);
    ls(&files, path, 0);
    destroy_files(&files);
    return 0;
}
int process_directory(const char* path, int show_header) {
    DIR* dir = opendir(path);
    if (!dir) {
        return Open_directory_error;
    }

    FileInfos files;
    init_file_infos(&files, 2, 0);

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == -1) {
            printf("FILE %s permission denied\n", full_path);
            continue;
        }

        char* link_target = NULL;
        if (S_ISLNK(st.st_mode)) {
            char buffer[PATH_MAX];
            ssize_t len = readlink(full_path, buffer, sizeof(buffer)-1);
            if (len != -1) {
                buffer[len] = '\0';
                link_target = buffer;
            }
        }

        insert_file_info(&files, entry->d_name, &st, link_target);
    }

    closedir(dir);

    qsort(files.data, files.size, sizeof(FileInfo), compare_files);
    ls(&files, path, show_header);
    destroy_files(&files);
    return 0;
}
