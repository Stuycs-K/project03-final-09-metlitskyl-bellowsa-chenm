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

int tree_transmit(char * path, int transmit_fd){
    printf(path);
    DIR * d;
    d = opendir( path );
    v_err(d==NULL?-1:0, "could not open dir", 1);

    struct dirent * entry;
    struct stat * stat_buffer = malloc(sizeof(struct stat));

    while(entry = readdir(d)){
        
        printf("%s\n",entry->d_name);
        if (entry->d_type == DT_DIR ){
            if(strcmp(entry -> d_name, ".") && strcmp(entry -> d_name, "..")){
                char new_path[(strlen(path) + 2 + strlen(entry->d_name))*sizeof(char)];
                sprintf(new_path, "%s/%s", path, entry->d_name);
                tree_transmit(new_path, transmit_fd);
            }
        }
        else{
            printf("transing...\n");
            write(transmit_fd, path, sizeof(path));
        }
    }
}
