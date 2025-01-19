#include "user.h"
#include "utils.h"
#include "file_transfer.h"
#include "user.h"

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int new_ft_user(char * name, int sound, char * ip, struct ft_user * user){
    strcpy(user->name, name);
    strcpy(user->ip, ip);
    user->sound = sound;

}

int init_client_config(char * program_name, struct ft_user * user, int refactor){
    int made_new_user = 0;
    
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
    
    if(mkdir_r != -1 || refactor){
        printf("---------------USER CONFIGURATION-------------------\n");
        printf("created your config dir at %s\n", path);
        strcat(path, "/user");

        int fd = open(path, O_CREAT |O_TRUNC| O_WRONLY, 0644);
        v_err(fd, "could not create usr config file", 1);
        //get username
        char username[1024];
        printf("set your username: ");
        fflush(stdout);
        fgets(username, sizeof(username), stdin);
        //remove \n
        username[strlen(username) - 1] = 0;
        
        char sound_yn;
        int sound = 0;
        printf("would you like sound effects [Y/n] WARNING sound effects will slow your program by 3sec: ");
        fflush(stdout);
        scanf("%c", &sound_yn);

        if(sound_yn == 'Y'){
            sound = 1;
        }

        char ip[256];
        printf("ip adress of server: ");
        fflush(stdout);
        
        scanf("%s", ip);

        printf("--------------------------------------\n\n");


        struct ft_user user;
        new_ft_user(username, sound, ip, &user);

        printf("IP %s\n", user.ip);
        write(fd, &user, sizeof(user));

        close(fd);

        made_new_user = 1;
    }
    else{
        strcat(path, "/user");
    }

    if(user){
        int fd = open(path, O_RDONLY, 0);

        read(fd, user, sizeof(struct ft_user));

        close(fd);
    }

    return made_new_user;
}