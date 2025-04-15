#include "solution.h"

int main(int argc, char *argv[]) {
    char **paths;
    int num_paths;

    if (argc > 1) {
        paths = argv + 1;
        num_paths = argc - 1;
    } else {
        static char *default_paths[] = {"."};
        paths = default_paths;
        num_paths = 1;
    }

    int err;
    for (int i = 0; i < num_paths; i++) {
        struct stat st;
        err = lstat(paths[i], &st);
        if (err == -1) {
            printf("FILE %s permission denied\n", paths[i]);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            err = process_directory(paths[i], num_paths > 1);
        } else {
            err = process_file(paths[i]);
        }
        switch (err) {
            case Null_pointer_error:
                printf("Null pointer error\n");
                return err;
            case Open_directory_error:
                printf("Open directory error\n");
                return err;
            default:
                continue;
        }
    }
    return 0;
}
