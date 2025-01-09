#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int v_err(int sigerr, char * msg, int _exit){
    if(sigerr < 0){
        perror(msg);
        if (_exit){
            exit(1);
        }
    }
    return sigerr;
}

