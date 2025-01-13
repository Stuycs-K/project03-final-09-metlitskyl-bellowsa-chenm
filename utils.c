#include "utils.h"
#include "file_transfer.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int v_err(int sigerr, char *msg, int _exit) {
    if (sigerr < 0) {
        perror(msg);
        if (_exit) {
            exit(1);
        }
    }
    return sigerr;
}

void err() {
    printf("ERROR %d: %s\n", errno, strerror(errno));
    exit(errno);
}

int get_repo_path(char * server_root, struct ft_init * init, char * target){
      char * username = init->user.name;
      char * repo_name = init->repo_name;

      sprintf(target, "%s/%s/%s", server_root, username, repo_name);
}