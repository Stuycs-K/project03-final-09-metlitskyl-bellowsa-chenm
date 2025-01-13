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

int main(int argc, char const* argv[]){
    
    if (argc < 3){
        perror("try using {pgrm name} {download/push} {directory-name}");
        return 1;
    }

    int client_fd = setup_client();
        
    printf("connected...\n");

    struct ft_init init;
    struct ft_user user;

    int made_new_user = init_client_config((char *)argv[0], &user);

    char repo_name[512];
    char repo_target[1050];
    get_repo_name_from_cwd(repo_name, sizeof(repo_name), repo_target);
    printf("repo name: %s, repo_traget: %s\n", repo_name, repo_target);
    exit(0);
    if(made_new_user){
        struct ft_init init_usr;
        new_ft_init(TR_AINIT, "", &user, &init_usr);
        write(client_fd, &init_usr, sizeof(struct ft_init));
    }

    if (!strcmp(argv[1], "download")){
        //init connection and ask for a transmission
        new_ft_init(TR_TRSMT, repo_name, &user, &init);
        write(client_fd, &init, sizeof(struct ft_init));

        recv_full_directory_contents(client_fd, "./");
        return 0;
    }
    else if(!strcmp(argv[1], "push")){
        //init connection and ask for a transmission
        new_ft_init(TR_RECV, (char *) argv[2], &user, &init);
        
        write(client_fd, &init, sizeof(struct ft_init));
        
        send_full_directory_contents(client_fd, (char *) argv[2]);

    }

    // closing the connected socket
    close(client_fd);
    return 0;
}
