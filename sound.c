#include "sound.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int play_song(char * path_to_program_dir, char * song, int delay){
    char bumble[strlen(path_to_program_dir) + strlen(song) + 20];
    sprintf(bumble, "%s/media/%s", path_to_program_dir, song);

    int kidid = fork();
    if(kidid == 0){
        int fd = open("/dev/null", O_WRONLY, 0);
        dup2(fd, fileno(stdout));
        dup2(fd,fileno(stderr));

        int r = execl("/bin/mpg123", "mpg123", bumble, (char *)NULL);
        perror("install mpg123 for full functionality...");
        exit(0);
    }
    sleep(delay);

    return kidid;
}