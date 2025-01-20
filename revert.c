#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "build.h"
#include "diff.h"
#include "add.h"
#include "commit.h"

void revert(char *tracked_dir, int commit_num, char *filename) {
    char dit_folder[MAX_FILEPATH] = "";
    char commit_folder[MAX_FILEPATH] = "";
    char staging_folder[MAX_FILEPATH] = "";
    populate_dit_folders(tracked_dir, dit_folder, commit_folder, staging_folder);
    
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    
    char filepath[MAX_FILEPATH] = "";
    strcat(filepath, cwd);
    strcat(filepath, "/");
    strcat(filepath, filename);
    
    int max_commit_number = get_max_commit_number(tracked_dir);
    
    //get original file from that commit num
    char *str = build_str(commit_num, commit_folder, filename);
//    printf("STR FROM THAT COMMIT NUM: %s\n", str);
    
    //write str to current file
    int out = open(filepath, O_WRONLY | O_TRUNC, 0644);
//    printf("FILEPATH: %s\n", filepath);
    
    int res = write(out, str, strlen(str));//plus 1??
    
    //add
    add(tracked_dir, filename, 1);
    
    //commit
    commit(tracked_dir);
}
