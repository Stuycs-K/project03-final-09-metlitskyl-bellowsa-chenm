#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

#include "client_module.h"
#include "build.h"
#include "add.h"
#include "commit.h"

#define VERSION "v1.0"

void print_usage(){
    printf("usage: dit <command> [<args>] version: " VERSION "\n\n"

           "These are common dit commands used in various situations:\n\n"

           "create a repo to push to: \n"
           "    init\n"
           "    download\n"
           "    pull\n\n"

           "remote (pull and push files from server): \n"
           "    push\n\n"

           "version control: \n"
           "    add: creates patch for specific file in staging folder\n"
           "    commit: renames staging folder to commit id\n"
           "    build: takes already downloaded .dit and builds files\n\n"
           
           );

}

int main(int argc, char * argv[]){
    if(argc == 1){
        print_usage();
        exit(0);
    }

    char * cmd = argv[1];

  


    if( !strcmp(cmd, "download") ||
        !strcmp(cmd, "push") ||
        !strcmp(cmd, "init") ){
        client(argc, argv);
        exit(0);
    }

    if(!strcmp(cmd,"pull")){
        // printf("about to make argv2\n");
        char * argv2[2];
        argv2[0] =  argv[0];
        argv2[1] = calloc(strlen("download")+1,sizeof(char));
        strcpy(argv2[1], "download");
        // printf("client\n");
        client(2, argv2);
        // printf("build\n");
        build("./");
        free(argv2[1]);
        exit(0);
    }

    if(!strcmp(cmd, "build")){
        build("./");
        exit(0);
    }
    if(!strcmp(cmd, "add")){
        add("./", argv[2]);
        exit(0);
    }
    if(!strcmp(cmd, "commit")){
        commit("./");
        exit(0);
    }
    
    printf("cmd not found :(\n");
    exit(1);
    
    
}