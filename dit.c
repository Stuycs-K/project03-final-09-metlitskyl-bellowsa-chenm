#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

// #include "client_module.h"
#include "build.h"
#include "status.h"
#include "add.h"
#include "commit.h"
#include "download.h"
#include "push.h"

#include "user.h"

#include "file_transfer.h"

#define VERSION "v1.3"

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

    if( !strcmp(cmd, "download")){
        download(argc, argv);
        exit(0);
    }

    if(!strcmp(cmd, "push") ){
        push(argc, argv);
        exit(0);
    }

    if(!strcmp(cmd, "init") ){
        init(argc, argv);
        exit(0);
    }

    if(!strcmp(cmd,"pull")){
        download(argc, argv);
        build("./");
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
    if(!strcmp(cmd, "status")){
        status("./");
        exit(0);
    }
    
    if(!strcmp(cmd, "user")){
        struct ft_user user;
        init_client_config(argv[0],&user, 1);
        printf("creating server user_profile...\n");

        int client_fd = setup_client(user.ip);
        struct ft_init init_usr;
        new_ft_init(TR_AINIT, "", &user, &init_usr);
        write(client_fd, &init_usr, sizeof(struct ft_init));
        exit(0);
    }
    printf("cmd not found :(\n");
    exit(1);
}

