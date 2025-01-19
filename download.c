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

int download(int argc, char * argv[]){

    char path_to_programdir[1024];

    realpath(argv[0], path_to_programdir);
    path_to_programdir[strlen(path_to_programdir) - strlen(__FILE__)  + 1] = 0;

    int client_fd = setup_client();
    printf("connected...\n");

    struct ft_init init;
    struct ft_user user;

    init_client_config((char *)argv[0], &user);

    char repo_name[512];
    char repo_name_dit[512];
    char repo_target[1050];

    get_repo_name_from_cwd(repo_name, sizeof(repo_name), repo_name_dit, repo_target);

    // if(made_new_user){
    //     struct ft_init init_usr;
    //     new_ft_init(TR_AINIT, "", &user, &init_usr);
    //     write(client_fd, &init_usr, sizeof(struct ft_init));
    // }

   
    //init connection and ask for a transmission

    //this stuff is for sound
    int kidid;
    if (user.sound){
        kidid = play_song(path_to_programdir,"kesh.mp3", 3);
    }

    new_ft_init(TR_TRSMT, repo_name, &user, &init);
    write(client_fd, &init, sizeof(struct ft_init));
    
    mkdir(".dit",0744); //just insure that there is a .dit
    recv_full_directory_contents(client_fd, ".");

    //kill the mpg123 if sound is on
    if(user.sound){
        kill(kidid, SIGKILL);
    }

    close(client_fd);
    printf("done!\n");
    return 0;
}