#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int v_err(int sigerr, char *msg, int _exit) {
    if (sigerr < 0) {
        perror(msg);
        if (_exit) {
            exit(1);
        }
    }
    return sigerr;
}

void err() {
    printf("ERROR %d: %s\n", errno, strerror(errno));
    exit(errno);
}
