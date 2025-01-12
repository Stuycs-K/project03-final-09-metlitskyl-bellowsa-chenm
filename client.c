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
    
    if (argc < 3){
        perror("try using {pgrm name} {download/push} {directory-name}");
        return 1;
    }

    int client_fd = setup_client();
        
    printf("connected...\n");

    struct ft_init init;
    struct ft_user user;

    init_client_config((char *)argv[0], &user);

    if (!strcmp(argv[1], "download")){
        //init connection and ask for a transmission
        new_ft_init(TR_TRSMT, "./test_dir", &user, &init);
        write(client_fd, &init, sizeof(struct ft_init));

        recv_full_directory_contents(client_fd, (char *)argv[2]);
        return 0;
    }
    else if(!strcmp(argv[1], "push")){
        //init connection and ask for a transmission
        new_ft_init(TR_RECV, "./server_dir", &user, &init);
        write(client_fd, &init, sizeof(struct ft_init));
        
        send_full_directory_contents(client_fd, (char *) argv[2]);

    }

    // closing the connected socket
    close(client_fd);
    return 0;
}
