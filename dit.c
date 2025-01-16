#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

#include "client_module.h"

void print_usage(){
    printf("usage: dit <command> [<args>]\n\n");
    printf("These are common dit commands used in various situations:\n\n");
    printf("create a repo to push to: \n");
    printf("    init\n\n");

    printf("remote (pull and push files from server): \n");
    printf("    download\n");
    printf("    push\n\n");

    printf("version control: \n");
    printf("    add: creates patch for specific file in staging folder\n");
    printf("    commit: renames staging folder to commit id\n");
    printf("    build: takes already downloaded .dit and builds files\n\n");

}

#define REMOTE 0
#define VERSION 1
#define CMD_NOT_FOUND 2

int what_related(char * cmd){
    if(!strcmp(cmd, "download") ||
       !strcmp(cmd, "push") ||
       !strcmp(cmd, "init")){

        return REMOTE;
    }

    return CMD_NOT_FOUND;
    //continue building this out
}

int main(int argc, char * argv[]){
    if(argc == 1){
        print_usage();
        exit(0);
    }

    char * cmd = argv[1];

    int category = what_related(cmd);

    if (category == CMD_NOT_FOUND){
        perror("cmd not found :(");
        exit(1);
    }

    if( category == REMOTE ){
        client(argc, argv);
    }
    
    
}