#include "utils.h"
#include "file_transfer.h"
#include "networking.h"
#include "user.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

int get_repo_path(char *server_root, struct ft_init *init, char *target) {
    char *username = init->user.name;
    char *repo_name = init->repo_name;

    sprintf(target, "%s/%s/%s", server_root, username, repo_name);
}

int get_repo_name_from_cwd(char *repo_name, int repo_name_size, char *repo_name_dit, char *repo_target) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char dit[1050];
    sprintf(dit, "%s/.dit", cwd);

    strcpy(repo_target, dit);

    char repo_draft_name[repo_name_size];

    memset(repo_draft_name, 0, repo_name_size);
    int lastIndex = strlen(cwd) - 1;
    int slash_i = -1;

    for (int i = lastIndex; i >= 0; i--) {
        if (cwd[i] == '/') {
            slash_i = i;
            break;
        }
    }

    for (int i = slash_i + 1; i <= lastIndex; i++) {
        repo_draft_name[i - slash_i - 1] = cwd[i];
    }

    strcpy(repo_name, repo_draft_name);
    sprintf(repo_name_dit, "./%s/.dit", repo_draft_name);
}

int get_base_name(char *path, char *target) {
    printf("\n-------------get base name-----------------\n");
    printf("%s\n", path);
    if (path[0] != '/') {
        strcpy(target, path);
        return 0;
    }
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    strcpy(target, path + strlen(cwd));
    printf("\n-------------end base name-----------------\n");

    return 1;
}

void populate_dit_folders(char *tracked_dir, char *dit_folder, char *commit_folder, char *staging_folder) {
    strcat(dit_folder, tracked_dir);
    strcat(dit_folder, ".dit/");

    strcat(commit_folder, dit_folder);
    strcat(commit_folder, "commits/");

    strcat(staging_folder, dit_folder);
    strcat(staging_folder, "staging/");
}

int get_max_commit_number(char *tracked_dir) {
    char dit_folder[MAX_FILEPATH] = "";
    char commit_folder[MAX_FILEPATH] = "";
    char staging_folder[MAX_FILEPATH] = "";
    populate_dit_folders(tracked_dir, dit_folder, commit_folder, staging_folder);

    printf("Tracked Dit Folder : |%s|\n", tracked_dir);

    // 2. go through git tree commits folder

    // get latest (highest #) commit
    DIR *d;
    d = opendir(commit_folder);
    if (d == 0) {
        err();
    }

    int max_commit_number = -1;

    struct dirent *entry = NULL;
    while ((entry = readdir(d))) {
        if (entry->d_type != DT_DIR) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        int commit_number = -1;
        sscanf(entry->d_name, "%d", &commit_number);

        if (commit_number > max_commit_number) {
            max_commit_number = commit_number;
        }
    }

    return max_commit_number;
}

void new_client_session(char ** argv, struct client_session * cs){

    int made_new_user = init_client_config((char *)argv[0], &cs->user);
    
    realpath(argv[0], cs->path_to_programdir);
    cs->path_to_programdir[strlen(cs->path_to_programdir) - strlen("dit.c")  + 1] = 0;

    cs->client_fd = setup_client(cs->user.ip);
    printf("connected...\n");


    get_repo_name_from_cwd(cs->repo_name, sizeof(cs->repo_name), cs->repo_name_dit, cs->repo_target);

    if(made_new_user){
        struct ft_init init_usr;
        new_ft_init(TR_AINIT, "", &cs->user, &init_usr);
        write(cs->client_fd, &init_usr, sizeof(struct ft_init));
    }
}