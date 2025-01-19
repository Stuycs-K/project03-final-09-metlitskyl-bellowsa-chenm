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

int init(int argc, char * argv[]){
    //init connection and ask for a transmission

    struct client_session * cs = malloc(sizeof(struct client_session));
    new_client_session(argv, cs);
    // this stuff is for sound
    //this is analogous to git init. It creates the .dir directory 
    //it also sends a create request to the server so the server has the dir created
    //without init, push crashes on the server side

    // this stuff is for sound
    int kidid;
    if (cs->user.sound){
        kidid = play_song(cs->path_to_programdir,"church.mp3", 3);
    }

    struct ft_init init;
    // create init struct and send it
    new_ft_init(TR_RINIT, cs->repo_name, &cs->user, &init);
    write(cs->client_fd, &init, sizeof(struct ft_init));

    // make the .dit directory
    int r = mkdir(".dit",0744);
    v_err(r, "err making .dit dir...", 0);

    r = mkdir(".dit/staging",0744);
    v_err(r, "err making staging dir...", 0);

    r = mkdir(".dit/commits",0744);
    v_err(r, "err making commits dir...", 0);


    //kill the mpg123 if sound is on
    if(cs->user.sound){
        kill(kidid, SIGKILL);
    }

    close(cs->client_fd);
    free(cs);
    printf("done!\n");
    return 0;
}