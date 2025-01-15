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

#define SOUND 1

int main(int argc, char const* argv[]){
    

    if (argc < 2){
        perror("try using {pgrm name} {download/push/init}");
        return 1;
    }

    char path_to_programdir[1024];
    if (SOUND){
        realpath(argv[0], path_to_programdir);
        path_to_programdir[strlen(path_to_programdir) - strlen(__FILE__)  + 1] = 0;
    }


    int client_fd = setup_client();
    printf("connected...\n");

    struct ft_init init;
    struct ft_user user;

    int made_new_user = init_client_config((char *)argv[0], &user);

    char repo_name[512];
    char repo_name_dit[512];
    char repo_target[1050];

    get_repo_name_from_cwd(repo_name, sizeof(repo_name), repo_name_dit, repo_target);
  
    if(made_new_user){
        struct ft_init init_usr;
        new_ft_init(TR_AINIT, "", &user, &init_usr);
        write(client_fd, &init_usr, sizeof(struct ft_init));
    }

    if (!strcmp(argv[1], "download")){
        //init connection and ask for a transmission
        new_ft_init(TR_TRSMT, repo_name, &user, &init);
        write(client_fd, &init, sizeof(struct ft_init));
        
        mkdir(".dit",0744); //just insure that there is a .dit
        recv_full_directory_contents(client_fd, ".");
        return 0;
    }
    else if(!strcmp(argv[1], "push")){
        //init connection and ask for a transmission
        int kidid;
        if (SOUND){
            char bumble[strlen(path_to_programdir) + 100];
            sprintf(bumble, "%s/media/bumble.mp3", path_to_programdir);

            kidid = fork();
            if(kidid == 0){
                int fd = open("/dev/null", O_WRONLY, 0);
                dup2(fd, fileno(stdout));
                dup2(fd,fileno(stderr));

                int r = execl("/bin/mpg123", "mpg123", bumble, (char *)NULL);
                perror("install mpg123 for full functionality...");
                exit(0);
            }
            sleep(3);
        }

        new_ft_init(TR_RECV, repo_name, &user, &init);
        write(client_fd, &init, sizeof(struct ft_init));
        
        send_full_directory_contents(client_fd, ".dit");

        if(SOUND){
            kill(kidid, SIGKILL);
        }

    }
    else if(!strcmp(argv[1], "init")){
        new_ft_init(TR_RINIT, repo_name, &user, &init);
        write(client_fd, &init, sizeof(struct ft_init));
        int r = mkdir(".dit",0744);
        v_err(r, "err making .dit dir...", 1);
    }
    // closing the connected socket
    close(client_fd);
    printf("done!\n");
    return 0;
}
