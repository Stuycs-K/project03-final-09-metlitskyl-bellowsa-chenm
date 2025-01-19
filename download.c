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

    struct client_session * cs = malloc(sizeof(struct client_session));
    new_client_session(argv, cs);

   
    //init connection and ask for a transmission

    //this stuff is for sound
    int kidid;
    if (cs->user.sound){
        kidid = play_song(cs->path_to_programdir,"kesh.mp3", 3);
    }

    struct ft_init init;
    new_ft_init(TR_TRSMT, cs->repo_name, &cs->user, &init);
    write(cs->client_fd, &init, sizeof(struct ft_init));
    
    mkdir(".dit",0744); //just insure that there is a .dit
    recv_full_directory_contents(cs->client_fd, ".");

    //kill the mpg123 if sound is on
    if(cs->user.sound){
        kill(kidid, SIGKILL);
    }

    close(cs->client_fd);
    free(cs);
    printf("done!\n");
    return 0;
}