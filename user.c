#include "user.h"
#include "utils.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int new_ft_user(char * name, struct ft_user * user){
    strcpy(user->name, name);
}

int init_client_config(char * program_name, struct ft_user * user){
    char path[1024];
    realpath(program_name, path);

    //now path has something like /blah/bloop/foo/prgm
    for(int c = strlen(path) -1; c >= 0; c--){
        if(path[c] == '/'){
            path[c] = 0;
            break;
        }
    }


    strcat(path, "/.client_config");
    int mkdir_r = mkdir(path, 0744);
    
    if(mkdir_r != -1){
        printf("---------------USER CONFIGURATION-------------------\n");
        printf("created your config dir at %s\n", path);
        strcat(path, "/user");

        int fd = open(path, O_CREAT | O_WRONLY, 0644);
        v_err(fd, "could not create usr config file", 1);
        //get username
        char username[1024];
        printf("set your username: ");
        fflush(stdout);
        fgets(username, sizeof(username), stdin);
        printf("--------------------------------------\n\n");

        //remove \n
        username[strlen(username) - 1] = 0;

        struct ft_user user;
        new_ft_user(username, &user);

        write(fd, &user, sizeof(user));

        close(fd);
    }
    else{
        strcat(path, "/user");
    }

    int fd = open(path, O_RDONLY, 0);

    read(fd, user, sizeof(struct ft_user));

    close(fd);
}