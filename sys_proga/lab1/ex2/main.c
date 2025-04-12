#include "solution.h"

int main(int argc, char *argv[]) {
    
    if (argc < 3) {
        print_usage();
        printf("Invalid input\n");
        return Invalid_input;
    }


    
    int file_count = 0;
    char *filenames[argc - 1];
    
   
    int op_pos = 1;
    while (op_pos < argc && argv[op_pos][0] != '-') {
        filenames[file_count++] = argv[op_pos++];
       
    }
    
    if (op_pos >= argc) {
        printf("Operation not specified\n");
        print_usage();
        return Invalid_input;
    }
    
    char *operation = argv[op_pos];
    
    if (strncmp(operation, "-xor", 4) == 0) {
        
        int n= atoi(operation + 4);
        if (n < 2 || n > 6) {
            printf("N must be between 2 and 6 for xor operation\n");
            return XORN_error;
        }
        
        if ((xor_files(filenames, file_count, n)) == File_error) {
            printf("File_error");
            return File_error;
        }
        // printf("XOR result (N=%d): ", n);
        // for (int i = 0; i < block_size; i++) {
        //     printf("%02x", block[i]);
        // }
        // printf("\n");
    } 
    else if (strcmp(operation, "-mask") == 0) {
        
        if (op_pos + 1 >= argc) {
            printf("Mask value not specified\n");
            return Mask_error;
        }
        char *mask_hex = argv[op_pos + 1];
        int res;
        if (strlen(mask_hex) > 8) {
            printf("Mask too long, maximum 8 hex digits\n");
            return Mask_error;
        }
        if ((res = count_with_mask(filenames, file_count, mask_hex)) == 1) {
            printf("File_error");
            return File_error;
        }
        
        printf("Numbers matching mask %s: %d\n", mask_hex, res);
    
    } 
    else if (strncmp(operation, "-copy", 5) == 0) {
        
        int n = atoi(operation + 5);
        int res;
        if (n <= 0) {
            printf("Number of copies must be positive\n");
            return Copy_error;
        }
        if ((res = create_copies(filenames, file_count, n)) == 1) {
            printf("Fork_error");
            return Fork_error;
        }
        printf("Created %d copies for each of %d files\n", n, file_count);
    }
    else if (strcmp(operation, "-find") == 0) {
        int res;
        
        if (op_pos + 1 >= argc) {
            printf("Search string not specified\n");
            return Find_error;
        }
        char *search_str = argv[op_pos + 1];
        switch (find_string(filenames, file_count, search_str))
        {
            case File_error:
                printf("File_error");
                return File_error;
                
            case Fork_error:
                printf("Fork_error\n");
                return Fork_error;
                
            case Wait_error:
                printf("Wait_error\n");
                return Wait_error;
                
            default:
                return 0;
                

        }
        
    }
    else {
        printf("Unknown operation\n");
        print_usage();
        return Unrecognized_command;
    }
    
    return 0;
}
