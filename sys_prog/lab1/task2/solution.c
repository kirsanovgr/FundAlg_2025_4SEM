#include "solution.h"

void* my_memmem(const void* haystack, size_t haystacklen,
                const void* needle, size_t needlelen) {
    if (needlelen == 0) return (void*)haystack;
    if (haystacklen < needlelen) return NULL;

    const char* h = haystack;
    const char* n = needle;

    for (size_t i = 0; i <= haystacklen - needlelen; i++) {
        if (memcmp(h + i, n, needlelen) == 0) {
            return (void*)(h + i);
        }
    }
    return NULL;
}

int process_xorN(char** files, int n_files, int N) {
    int block_size = (1 << N) / 8;
    for (int i = 0; i < n_files; i++) {
        FILE* fp = fopen(files[i], "rb");
        if (!fp) {
            fprintf(stderr, "Error opening: %s\n", files[i]);
            return FILE_ERROR;
        }

        unsigned int result = 0;

        size_t bytes_read;
        if (N == 2) {
            unsigned char xor_result = 0;
            unsigned char byte;
            while (fread(&byte, 1, 1, fp) == 1) {
                xor_result ^= (byte >> 4) & 0x0F;
                xor_result ^= byte & 0x0F;
            }
            result = xor_result;
        } else {
            unsigned char block;
            while ((bytes_read = fread(&block, 1, block_size, fp)) > 0) {
                if (bytes_read < (size_t) block_size) {
                    memset(fp + bytes_read, 0, block_size - bytes_read);
                }
                for (int j = 0; j < block_size; j++) {
                    result ^= block;
                }
            }
        }

        printf("%s: ", files[i]);
        printf("%04x", result);
        printf("\n");

        fclose(fp);
    }
    return OK;
}

int process_mask(char** files, int n_files, uint32_t mask) {
    for (int i = 0; i < n_files; i++) {
        FILE* fp = fopen(files[i], "rb");
        if (!fp) {
            fprintf(stderr, "Error opening: %s\n", files[i]);
            return FILE_ERROR;
        }

        unsigned char buffer[4];
        size_t bytes_read;
        unsigned int count = 0;

        while ((bytes_read = fread(buffer, 1, 4, fp))) {
            if (bytes_read < 4)  {
                memmove(buffer + 4 - bytes_read, buffer, bytes_read);
                memset(buffer, 0, 4 - bytes_read);
            }
            unsigned int value = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
            if ((value & mask) == mask) count++;
        }

        printf("File: %s Count: %u\n", files[i], count);
        fclose(fp);
    }
    return OK;
}

int process_copyn(char** files, int n_files, int N) {
    for (int i = 0; i < n_files; i++) {
        char src_path[PATH_MAX];
        if (!realpath(files[i], src_path)) {
            printf("Invalid path: %s\n", files[i]);
            return FILE_ERROR;
        }

        char* dot = strrchr(src_path, '.');
        char name[PATH_MAX] = {0};
        char ext[PATH_MAX] = {0};

        if (dot) {
            strncpy(name, src_path, dot - src_path);
            name[dot - src_path] = '\0';
            strcpy(ext, dot);
        } else {
            strcpy(name, src_path);
        }

        const size_t max_num_len = 10;
        const size_t safety_margin = 2;
        const size_t max_name_len = PATH_MAX - strlen(ext) - max_num_len - safety_margin;

        if (strlen(name) > max_name_len) {
            name[max_name_len] = '\0';
        }

        for (int j = 1; j <= N; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                char dest[PATH_MAX];
                int written = snprintf(dest, sizeof(dest), "%s_%d%s", name, j, ext);

                if (written < 0 || (size_t)written >= sizeof(dest)) {
                    printf("Path too long: %s\n", dest);
                    exit(FILE_ERROR);
                }

                FILE* src = fopen(src_path, "rb");
                if (!src) {
                    printf("Can't open source: %s\n", src_path);
                    exit(FILE_ERROR);
                }

                FILE* dest_fp = fopen(dest, "wb");
                if (!dest_fp) {
                    fclose(src);
                    printf("Can't create: %s\n", dest);
                    exit(FILE_ERROR);
                }

                char buffer[4096];
                size_t bytes_read;
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
                    size_t bytes_written = fwrite(buffer, 1, bytes_read, dest_fp);
                    if (bytes_written != bytes_read) {
                        printf("Write error to %s\n", dest);
                        fclose(src);
                        fclose(dest_fp);
                        exit(FILE_ERROR);
                    }
                }

                if (ferror(src)) {
                    printf("Read error from %s\n", src_path);
                    fclose(src);
                    fclose(dest_fp);
                    exit(FILE_ERROR);
                }

                fclose(src);
                fclose(dest_fp);
                exit(OK);
            }
        }
    }

    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && WEXITSTATUS(status) != OK) {
            printf("Copy process failed\n");
            return FILE_ERROR;
        }
    }
    return OK;
}

int process_find(char** files, int n_files, const char* search_str) {
    int found = 0;
    size_t len = strlen(search_str);
    if (len == 0) return INVALID_OPTIONS;

    for (int i = 0; i < n_files; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            FILE* fp = fopen(files[i], "r");
            if (!fp) {
                printf( "Can't open %s\n", files[i]);
                _exit(FILE_ERROR);
            }

            char* buf = malloc(len + 4096);
            if (!buf) {
                fclose(fp);
                _exit(MEMORY_LEAK);
            }

            size_t total = 0;
            int found_in_file = 0;
            while (!found_in_file) {
                size_t read = fread(buf + total, 1, 4096, fp);
                if (read == 0) break;

                total += read;
                if (my_memmem(buf, total, search_str, len)) {
                    found_in_file = 1;
                    printf("Found in: %s\n", files[i]);
                } else {
                    memmove(buf, buf + total - len + 1, len - 1);
                    total = len - 1;
                }
            }

            free(buf);
            fclose(fp);
            _exit(found_in_file ? OK : FILE_ERROR);
        }
    }

    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == OK) found = 1;
    }
    return found ? OK : FILE_ERROR;
}
