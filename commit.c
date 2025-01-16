#include "utils.h"

void commit(char *tracked_dir) {

    char dit_folder[MAX_FILEPATH] = "";
    char commit_folder[MAX_FILEPATH] = "";
    char staging_folder[MAX_FILEPATH] = "";
    populate_dit_folders(tracked_dir, dit_folder, commit_folder, staging_folder);

    int max_commit_number = get_max_commit_number(tracked_dir);

    printf("Max Commit Number: %d\n", max_commit_number);

    if (access(staging_folder, F_OK) != 0) {
        printf("Staging folder at |%s| does not exist.\n", staging_folder);
        exit(1);
    }

    // see if staging dir is empty
    DIR *d;
    d = opendir(staging_folder);
    if (d == 0) {
        err();
    }

    int file_found = 0;

    struct dirent *entry = NULL;
    while ((entry = readdir(d))) {
        if (entry->d_type != DT_REG) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        file_found = 1;
        break;
    }

    if (file_found) {
        char new_commit_str[50] = "";
        sprintf(new_commit_str, "%d", max_commit_number + 1);
        char new_commit_dir[MAX_FILEPATH] = "";
        strcat(new_commit_dir, commit_folder);
        strcat(new_commit_dir, new_commit_str);
        printf("Commiting to |%s|\n", new_commit_dir);
        // move staging patches to new commit folder
        int rename_status = rename(staging_folder, new_commit_dir);
        if (rename_status == -1) {
            err();
        }

        // make a new empty staging folder
        int mk_status = mkdir(staging_folder, 0755);
        if (mk_status == -1) {
            err();
        }

        printf("Commit |%s| created successfully.\n", new_commit_str);
    } else {
        printf("Nothing to commit! Staging folder is empty!\n");
    }
}

int main() {
    commit("dit_test_dir/");
}