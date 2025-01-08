#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILEPATH 512

#define MODE_TOUCH 0
#define MODE_MODIFY 1 // modifying TEXT inside a file
#define MODE_REMOVE 2

#define MODE_PLUS 3
#define MODE_MINUS 4

int v_err(int sigerr, char *msg, int _exit) {
    if (sigerr < 0) {
        perror(msg);
        if (_exit) {
            exit(1);
        }
    }
    return sigerr;
}

struct patch {
    char filepath[MAX_FILEPATH];
    int MODE;
    size_t memory_size;
    void *memory; // raw memory, used for chars in mode_touch, used for our diff format in mode_modify, 1 byte for mode_remove
};