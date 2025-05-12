#include "solution.h"

void print_usage() {
    printf( "Usage: file_processor file1 [file2...] operation [args]\n");
    printf( "Operations:\n");
    printf( "  -xorN    - XOR operation with block size 2^N bits (N=2-6)\n");
    printf( "  -mask <hex> - count 32-bit numbers matching mask\n");
    printf( "  -copyN   - create N copies of each file\n");
    printf( "  -find <string> - search for string in text files\n");
}


int xor_files(char** files, int n_files, int n) {
    int block_size = (1 << n) / 8;
    for (int i = 0; i < n_files; i++) {
        FILE* fp = fopen(files[i], "rb");
        if (!fp) {
            fprintf(stderr, "Error opening: %s\n", files[i]);
            return File_error;
        }

        unsigned int result = 0;

        size_t bytes_read;
        if (n == 2) {
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
    return 0;
}

int count_with_mask(char **filenames, int file_count, const char *mask_hex) {
    uint32_t mask = strtoul(mask_hex, NULL, 16);
    uint32_t file_mask;
    int count = 0;
    
    for (int i = 0; i < file_count; i++) {
        FILE *file = fopen(filenames[i], "rb");
        if (!file) {
            return 1;
        }
        
        while (fread(&file_mask, sizeof(uint32_t), 1, file)) {
            if ((file_mask & mask) == mask) {
                count++;
            }
        }
        
        if (!feof(file)) {
            return 1;
        }
        
        fclose(file);
        return count;
    }
    
}

int copy_file(const char *filename, int copy_num) {
    char new_filename[256];
    snprintf(new_filename, sizeof(new_filename), "%s.%d", filename, copy_num);
    
    FILE *src = fopen(filename, "rb");
    FILE *dst = fopen(new_filename, "wb");
    if (!src || !dst) {
        fclose(src);
        fclose(dst);
        return 1;
    }
    
    
    uint8_t buffer[PATH_MAX];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, bytes_read, dst) != bytes_read) {
            fclose(src);
            fclose(dst);
            return 1;
        }
    }
    
    fclose(src);
    fclose(dst);
    return 0;
}

int create_copies(char **filenames, int file_count, int n) {
    pid_t pids[file_count * n];
    int pid_count = 0;
    
    for (int i = 0; i < file_count; i++) {
        for (int j = 1; j <= n; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                copy_file(filenames[i], j);
                
            } else if (pid > 0) {
                pids[pid_count++] = pid;
            } else {
                return 1;
            }
        }
    }
    
    for (int i = 0; i < pid_count; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    
    return 0;
}

int search_in_file(const char *filename, const char *search_str) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    char line[MAX_STRING_LEN];
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, search_str)) {
            found = 1;
            break;
        }
    }
    
    fclose(file);
    return found;
}

int find_string(char **filenames, int file_count, const char *search_str) {
    int pipes[file_count][2];
    pid_t pids[file_count];
    int found_any = 0;

    // Создаем pipe для каждого дочернего процесса
    for (int i = 0; i < file_count; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe failed");
            return -1;
        }
    }

    for (int i = 0; i < file_count; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Дочерний процесс
            close(pipes[i][0]); // Закрываем ненужный конец pipe
            
            int result = search_in_file(filenames[i], search_str);
            write(pipes[i][1], &result, sizeof(result));
            close(pipes[i][1]);
            
            return 0; // Выходим из дочернего процесса
        } 
        else if (pids[i] > 0) {
            // Родительский процесс
            close(pipes[i][1]); // Закрываем ненужный конец pipe
        } 
        else {
            perror("fork failed");
            return -1;
        }
    }

    // Читаем результаты из pipe
    for (int i = 0; i < file_count; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        
        int result;
        read(pipes[i][0], &result, sizeof(result));
        close(pipes[i][0]);
        
        if (result == 1) {
            printf("Found in file: %s\n", filenames[i]);
            found_any = 1;
        } 
        else if (result == -1) {
            printf("Error reading file: %s\n", filenames[i]);
        }
    }

    if (!found_any) {
        printf("String '%s' not found in any of the files\n", search_str);
    }
    
    return 0;
}