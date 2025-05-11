#include "main.h"


// ./main file.txt xorN

statuses xorN(char** argv, int argc, unsigned char** ans, size_t* len) {
    FILE* in[argc - 2];
    for (size_t i = 1; i < argc - 1; i++) {
        in[i - 1] = fopen(argv[i], "rb");
        if (in[i - 1] == NULL) {
            return OPEN_PROBLEM;
        }
    }
    char* flag = argv[argc - 1];
    int n = flag[3] - '0';
    if (!(n >= 2 && n <= 6)) {
        return INCORRECT_ARGUMENTS;
    }
    if (n != 2) {
        size_t block_bits = 1 << n;  
        size_t block_size = block_bits / 8;
        size_t count = 0;
        *len = block_size;
        // обычный массив будет храниться в стеке ФУНКЦИИ, поэтому после заврешения память невалид
        unsigned char* res =(unsigned char*) calloc(block_size, sizeof(unsigned char));
        if (res == NULL) {
            return ALLOC_ERROR;
        }
        unsigned char block[block_size];
        memset(block, 0, block_size);
        // по файлам
        for (size_t i = 0; i < argc - 2; i++) {
            // по байтам файла
            while(fread(&block[count], 1, 1, in[i]) == 1) {
                count++;
                // если количество пройденных блоков равно нужному
                if (count == block_size) {
                    for (size_t j = 0; j < block_size; j++) {
                        //printf("0x%02X ^ ", res[j]);
                        res[j] = res[j] ^ block[j];
                        //printf("0x%02X = ", block[j]);
                        //printf("0x%02X     ", res[j]);
                    }
                    //printf("\n");
                    count = 0;
                }

            }
            if (count != 0) {
                memset(block + count, 0, block_size - count);
                for (size_t j = 0; j < block_size; j++) {
                    res[j] = res[j] ^ block[j];
                }
            }
        }
        *ans = res;
    }
    // отдельно для по 4 бита
    if (n == 2) {
        size_t count = 0;
        unsigned char byte = 0;
        unsigned char* res =(unsigned char*) calloc(1, sizeof(unsigned char));
        *len = 1;
        if (res == NULL) {
            return ALLOC_ERROR;
        }
        // по файлам
        for (size_t i = 0; i < argc - 2; i++) {
            // по байтам файла
            while(fread(&byte, 1, 1, in[i]) == 1) {
                // первые 4
                unsigned char high = (byte >> 4) & 0x0F;
                // вторые 4
                unsigned char low = byte & 0x0F;
                (*res) = ((*res) ^ high) ^ low;

            }
            fclose(in[i]);
        }
        *ans = res;

    }
    return OK;


}

statuses copy(char** argv, int argc) {
    FILE* in[argc - 2];
    char* number = argv[argc - 1] + 4;
    int num = atoi(number);
    pid_t pid = getpid();
    for (size_t i = 1; i < argc - 1; i++) {
        if (pid > 0) {
            pid = fork();
            if (pid < 0) {
                exit(FORK_ERROR);
            }
        }
        if (pid == 0) {
            in[i - 1] = fopen(argv[i], "rb");
            if (in[i - 1] == NULL) {
                exit(OPEN_PROBLEM);
            }
            char buffer[512];
            size_t count;
            FILE* copies[num];
            for (size_t j = 0; j < num; j++) {
                char* full_name = strdup(argv[i]);
                char str[FILENAME_MAX];
                char* name = strtok(full_name, ".");
                char* raz = strtok(NULL, " ");
                snprintf(str,sizeof(str), "%s%lu.%s", name, j + 1, raz);
                free(full_name);
                copies[j] = fopen(str, "wb");
                if (copies[j] == NULL) {
                    exit(OPEN_PROBLEM);
                }
            }
            while((count = fread(buffer, 1, sizeof(buffer), in[i - 1])) > 0) {
                for (size_t j = 0; j < num; j++) {
                    fwrite(buffer, 1, count, copies[j]);
                }
            }
            for (size_t j = 0; j < num; j++) {
                fclose(copies[j]);
            }
            fclose(in[i - 1]);
            // для дочерних
            exit(OK);
        }

    }
    int exit_code;
    int status;
    statuses ans;
    for(size_t i = 0; i < argc - 2; i++) {
        wait(&exit_code);
        status = WEXITSTATUS(exit_code);
        if (status == OPEN_PROBLEM) {
            ans = OPEN_PROBLEM;
        }
        else if (status == FORK_ERROR) {
            ans = FORK_ERROR;
        }
    }
    return ans;
    
    

}


statuses find(char** argv, int argc, char*** finds, size_t* len) {
    FILE* in[argc - 3];
    char* needle = argv[argc - 1];
    pid_t procces_mass[argc - 3];
    pid_t pid = getpid();
    for (size_t i = 1; i < argc - 2; i++) {
        if (pid > 0) {
            pid = fork();
            procces_mass[i - 1] = pid;
            if (pid < 0) {
                exit(FORK_ERROR);
            }
        }
        if (pid == 0) {
            in[i - 1] = fopen(argv[i], "rb");
            if (in[i - 1] == NULL) {
                exit(OPEN_PROBLEM);
            }
            char buffer[512];
            size_t count;
            while ((count = fread(buffer, 1, sizeof(buffer), in[i - 1])) > 0) {
                //printf("%s\n", buffer);
                buffer[count] = 0;
                
                if (strstr(buffer, needle)) {
                    fclose(in[i - 1]);
                    exit(FIND);
                }
                if (feof(in[i - 1])) {
                    break;
                }   
                // граница буфера
                fseek(in[i - 1], -(strlen(needle) - 1), SEEK_CUR);
            }
            fclose(in[i - 1]);
            // для дочерних
            exit(OK);
        }

    }
    char** files = (char**) calloc(argc - 3, sizeof(char *));
    int exit_code;
    int status;
    statuses ans = OK;
    (*len) = 0;
    for(size_t i = 0; i < argc - 3; i++) {
        waitpid(procces_mass[i], &exit_code, 0);
        status = WEXITSTATUS(exit_code);
        if (status == FIND) {
            files[i] = realpath(argv[i + 1], NULL);
            (*len)++;
            ans = FIND;
        }
        else if (status == OK) {
            files[i] = NULL;
        }
        else if (status == OPEN_PROBLEM) {
            ans = OPEN_PROBLEM;
        }
        else if (status == FORK_ERROR) {
            ans = FORK_ERROR;
        }
    }
    (*finds) = files;
    return ans;
    
    

}


statuses mask(char** argv, int argc, size_t* ans) {
    FILE* in[argc - 3];
    for (size_t i = 1; i < argc - 2; i++) {
        in[i - 1] = fopen(argv[i], "rb");
        if (in[i - 1] == NULL) {
            return OPEN_PROBLEM;
        }
    }
    char* mask_str = argv[argc - 1];
    unsigned int mask = strtol(mask_str, NULL, 16);
    unsigned int current_num;
    (*ans) = 0;
    for (size_t i = 0; i < argc - 3; i++) {
        // по 4 байта из файла
        while(fread(&current_num, sizeof(unsigned int), 1, in[i]) == 1) {
            if ((current_num & mask) == mask) {
                //printf("0x%08X\n", current_num);
                (*ans)++;
            }
        }
        fclose(in[i]);
    }
    return OK;
        
}

statuses Task(int argc, char** argv) {
    if (argc < 3) {
        return INCORRECT_ARGUMENTS;
    }
    char* flag = argv[argc - 1];
    char* second_flag = argv[argc - 2];
    statuses result;
    if (flag[0] == 'x' && flag[1] == 'o' && flag[2] == 'r') {
        unsigned char* ans;
        size_t len;
        result = xorN(argv, argc, &ans, &len);
        if (result != OK) {
            return result;
        }
        if (len != 1) {

            for (size_t i = 0; i < len; i++) {
                printf("0x%02X ", ans[i]);
            }
        }
        else {
            printf("0x%X", *ans);
        }
        printf("\n");

    }
    else if(flag[0] == 'c' && flag[1] == 'o' && flag[2] == 'p' && flag[3] == 'y') {
        result = copy(argv, argc);
        if (result != OK) {
            return result;
        }

    }
    else if (second_flag[0] == 'm' && second_flag[1] == 'a' && second_flag[2] == 's' && second_flag[3] == 'k') {
        size_t ans;
        result = mask(argv, argc, &ans);
        if (result != OK) {
            return result;
        }
        printf("%lu\n", ans);
        

    }
    else if (second_flag[0] == 'f' && second_flag[1] == 'i' && second_flag[2] == 'n' && second_flag[3] == 'd') {
        char** ans;
        size_t len;
        result = find(argv, argc, &ans, &len);
        if (!(result == OK || result == FIND)) {
            return result;
        }
        if (len != 0) {
            for (size_t i = 0; i < argc - 3; i++) {
                if (ans[i] != NULL) {
                    printf("%s\n", ans[i]);
                    free(ans[i]);
                }
            }
        }
        else {
            printf("no needle in files\n");
        }
        free(ans);


    }
    return OK;


}

int main(int argc, char** argv) {
    switch(Task(argc, argv)) {
        case OPEN_PROBLEM:
            printf("open problem\n");
            return OPEN_PROBLEM;
        case INCORRECT_ARGUMENTS:
            printf("incorrect arguments\n");
            return INCORRECT_ARGUMENTS;
        case ALLOC_ERROR:
            printf("alloc error\n");
            return ALLOC_ERROR;
    }
}