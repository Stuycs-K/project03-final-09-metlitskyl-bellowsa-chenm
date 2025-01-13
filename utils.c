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

int get_repo_name_from_cwd(char * repo_name, int repo_name_size, char * repo_target){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char dit[1050];
    sprintf(dit, "%s/.dit", cwd);

    strcpy(repo_target, dit);

    char repo_draft_name[repo_name_size];

    memset(repo_draft_name, 0, repo_name_size);
    int lastIndex = strlen(cwd) -1;
    int slash_i = -1;

    for(int i = lastIndex; i>=0; i--){
        if(cwd[i] == '/'){
           slash_i = i;
           break;
        }
    }

    for (int i = slash_i; i<lastIndex;i++){
        repo_draft_name[i-slash_i] = cwd[i];
    }

    sprintf(repo_name, "./%s/.dit", repo_draft_name);

}