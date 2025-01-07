#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

int v_err(int sigerr, char * msg, int _exit){
    if(sigerr < 0){
        perror(msg);
        if (_exit){
            exit(1);
        }
    }
    return sigerr;
}