#include "error_handler.h"

int throw_err(errcode err) {
    char* err_msg;

    switch(err) {
        case OUT_OF_BOUNDS:
            err_msg = "Option is out of allowed bounds.";
            break;
        case NOT_A_NUMBER:
            err_msg = "Number was expected, not a number got instead.";
            break;
        case INCORRECT_ARGUMENTS:
            err_msg = "Entered arguments are not valid.";
            break;
        case INCORRECT_OPTION:
            err_msg = "Entered option is not supported.";
            break;
        case MEMORY_NOT_ALLOCATED:
            err_msg = "Memory was not allocated";
            break;
        case FILE_ERROR:
            err_msg = "Failed to open file.";
            break;
        case INCORRECT_INPUT_DATA:
            err_msg = "Input data is not valid.";
            break;
        case DIVISION_BY_ZERO:
            err_msg = "Trying to divide by zero.";
            break;
        case SEMAPHORE_ERROR:
            err_msg = "Semaphore error has occurred.";
            break;
        case SOCKET_ERROR:
            err_msg = "Socket error has occurred.";
            break;
        case MESSAGE_QUEUE_ERROR:
            err_msg = "Message queue error has occurred.";
            break;
        case FORK_ERROR:
            err_msg = "Fork error has occurred.";
            break;
        default:
            err_msg = "An unknown message has occurred.";
            break;
    }

    fprintf(stderr, "%s\n", err_msg);
    return (err);
}
