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

/*Transmit a file*/
int transmit_file(int transmit_fd, char * path, struct dirent * entry){
    struct file_transfer ft;
    if(entry){
        ft.mode = (entry->d_type == DT_REG) ? TR_FILE : TR_DIR;
        sprintf(ft.path, "%s/%s", path, entry->d_name);
    }
    else{
        ft.mode = TR_DIR;
        sprintf(ft.path, "%s", path);
    }
    printf("sending %s\n",ft.path);
    // printf("mode: %d\n", mode);
    write(transmit_fd, &ft, sizeof(struct file_transfer));
    return 0;
}

/*tree transmit goes through the file system at dir path
and for each file/dir transmits the name to the transmit_fd.
This serves to provide the info to build the correct file system.
File contents will also be sent with this fxn.*/
int tree_transmit(char * path, int transmit_fd){
    DIR * d;
    d = opendir( path );
    v_err(d==NULL?-1:0, "could not open dir", 1);

    struct dirent * entry;
    struct stat * stat_buffer = malloc(sizeof(struct stat));

    while(entry = readdir(d)){
        usleep(1);
        
        if(strcmp(entry -> d_name, ".") && strcmp(entry -> d_name, "..")){
            transmit_file(transmit_fd, path, entry);
            
            if (entry->d_type == DT_DIR ){
                char new_path[(strlen(path) + 2 + strlen(entry->d_name))*sizeof(char)];
                sprintf(new_path, "%s/%s", path, entry->d_name);
                tree_transmit(new_path, transmit_fd);
            }
        }
    }
        
    closedir(d);
}
