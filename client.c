#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//socket stuff
#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>

#include <fcntl.h>

//for mkdir
#include <sys/stat.h>

#include "utils.h"
#include "file_transfer.h"
#include "networking.h"

#define WORKING_DIR "/home/abellows50/systems/project03-final-09-metlitskyl-bellowsa-chenm/test"

int main(int argc, char const* argv[]){
    
    int client_fd = setup_client();
        
    printf("connected...\n");

    recv_full_directory_contents(client_fd, WORKING_DIR);
    // closing the connected socket
    close(client_fd);
    return 0;
}
