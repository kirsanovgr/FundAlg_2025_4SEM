#include "solution.h"

Flags parse_flag(char* flag_str) {
    if (strncmp(flag_str, "xor", 3) == 0) return XOR;
    if (strcmp(flag_str, "mask") == 0) return MASK;
    if (strncmp(flag_str, "copy", 4) == 0) return COPY;
    if (strcmp(flag_str, "find") == 0) return FIND;
    return -1;
}

void print_error(int error_code) {
    switch (error_code) {
        case OK:
            break;
        case INVALID_OPTIONS:
            printf("Invalid command line options\n");
            break;
        case MEMORY_LEAK:
            printf("Memory allocation error\n");
            break;
        case FILE_ERROR:
            printf("File operation error\n");
            break;
        case OVERFLOW_ERROR:
            printf("Size overflow detected\n");
            break;
        default:
            printf("Unknown error\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Invalid Options\n");
        return INVALID_OPTIONS;
    }

    int flag_pos = -1;
    for (int i = 1; i < argc; i++) {
        if (parse_flag(argv[i]) != (Flags)-1) {
            flag_pos = i;
            break;
        }
    }

    if (flag_pos == -1 || flag_pos == 1) {
        printf("Invalid flag\n");
        return INVALID_OPTIONS;
    }

    int n_files = flag_pos - 1;
    char** files = argv + 1;
    char* flag_str = argv[flag_pos];
    Flags flag = parse_flag(flag_str);

    int result = OK;

    switch (flag) {
        case XOR: {
            int N = atoi(flag_str + 3);
            if (N < 2 || N > 6) {
                printf("Invalid N for xor (2-6)\n");
                result = INVALID_OPTIONS;
                break;
            }
            result = process_xorN(files, n_files, N);
            break;
        }
        case MASK: {
            if (flag_pos + 1 >= argc) {
                printf("Missing mask value\n");
                result = INVALID_OPTIONS;
                break;
            }
            char* end;
            uint32_t mask = strtoul(argv[flag_pos + 1], &end, 16);
            if (*end != '\0') {
                printf("Invalid mask format\n");
                result = INVALID_OPTIONS;
                break;
            }
            result = process_mask(files, n_files, mask);
            break;
        }
        case COPY: {
            int N = atoi(flag_str + 4);
            if (N <= 0) {
                printf("Invalid copy count\n");
                result = INVALID_OPTIONS;
                break;
            }
            result = process_copyn(files, n_files, N);
            break;
        }
        case FIND: {
            if (flag_pos + 1 >= argc) {
                printf("Missing search string\n");
                result = INVALID_OPTIONS;
                break;
            }
            result = process_find(files, n_files, argv[flag_pos + 1]);
            if (result == FILE_ERROR) {
                printf("String not found in any files\n");
                result = OK;
            }
            break;
        }
        default:
            printf("Unknown flag\n");
            result = INVALID_OPTIONS;
    }

    if (result != OK) {
        print_error(result);
    }
    return result;
}
