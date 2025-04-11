#include "solution.h"

int main() {
    int res = dialog_manager();
    switch (res) {
        case Memory_leak:
            printf("Memory leak");
            return Memory_leak;
        case Invalid_code:
            printf("Invalid secret code");
            return Invalid_code;
        case File_error:
            printf("File error");
            return File_error;
        case Null_pointer_error:
            printf("Null pointer error");
            return Null_pointer_error;
        default:
            return 0;
    }
}
