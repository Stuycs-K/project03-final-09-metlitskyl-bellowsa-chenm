#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

//socket stuff
#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/socket.h> 

#include <signal.h>

#include <netdb.h>

#include <fcntl.h>

//for mkdir
#include <sys/stat.h>

#include "utils.h"
#include "file_transfer.h"
#include "networking.h"

#include "sound.h"

void print_usage(){
    printf("usage: dit <command> [<args>]\n\n");
    printf("These are common dit commands used in various situations:\n\n");
    printf("create a repo to push to: \n");
    printf("    init\n\n");

    printf("download from a remote repo. you need to be in the repo: \n");
    printf("    download\n\n");

    printf("push to a remote repo from inside repo:\n");
    printf("    push");

    //LENNY AND MATTHEW ADD YOU CMDS HERE
}

int main(int argc, char * argv[]){
    if(argc == 1){
        print_usage();
    }

}