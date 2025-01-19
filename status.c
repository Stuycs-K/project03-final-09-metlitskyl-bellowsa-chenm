#include "diff.h"
#include "patch.h"
#include "utils.h"


void status(char *tracked_dir){
    printf("Dit status...\n");

    char dit_folder[MAX_FILEPATH] = "";
    char commit_folder[MAX_FILEPATH] = "";
    char staging_folder[MAX_FILEPATH] = "";
    populate_dit_folders(tracked_dir, dit_folder, commit_folder, staging_folder);
    int max_commit_number = get_max_commit_number(tracked_dir);

    char **filenames_in_history = calloc(MAX_FILES, sizeof(char *));
    int *does_file_still_exist_in_dit_tree = calloc(MAX_FILES, sizeof(int));

    int num_of_files_in_history = get_files_in_tree(max_commit_number, commit_folder, filenames_in_history, does_file_still_exist_in_dit_tree);

    for (int i = 0; i < num_of_files_in_history; i++) {
        if (!does_file_still_exist_in_dit_tree[i]) {
            continue; // skip if file got deleted from dit tree
        }
    }
}

int get_files_in_tree(int max_commit_number, char *commit_folder, char **filenames_in_history, int *does_file_still_exist_in_dit_tree){
    int num_of_files_in_history = 0;


    printf("Loop to see if file has been net added or not...\n");

    for (int i = 0; i <= max_commit_number; i++) {
        // this loop sees if the file has been net added or removed at the very present
        char specific_commit_folder[MAX_FILEPATH] = "";

        char folder_name_str[50] = "";
        sprintf(folder_name_str, "%d", i);

        strcat(specific_commit_folder, commit_folder);
        strcat(specific_commit_folder, folder_name_str);

        // printf("Specific commit folder: |%s|\n", specific_commit_folder);
        // open dir and apply every patch
        DIR *commit_dir;
        commit_dir = opendir(specific_commit_folder);
        if (commit_dir == 0) {
            err();
        }
        struct dirent *diff_entry = NULL;
        // printf("Directories: \n");
        while ((diff_entry = readdir(commit_dir))) {
            if (diff_entry->d_type != DT_REG) {
                continue;
            }
            if (strcmp(diff_entry->d_name, ".") == 0 || strcmp(diff_entry->d_name, "..") == 0) {
                continue;
            }

            char patch_full_path[MAX_FILEPATH] = "";
            sprintf(patch_full_path, "%s/%s", specific_commit_folder, diff_entry->d_name);

            // patch name doesn't neccesarily have to be the file name (in case dups/nested)
            Patch *p = read_patch(patch_full_path);

            printf("Read patch |%s|!\n", patch_full_path);

            // p-> filepath is just the name of the file inside the tracked dir alr
            int index_in_filename_list = find_index_in_filename_list(filenames_in_history, num_of_files_in_history, p->filepath);
            if (p->mode == MODE_REMOVE) {
                // for a remove patch to exist, means previous commit must have added it, so just delete
                does_file_still_exist_in_dit_tree[num_of_files_in_history] = 0;
                continue;
            }

            if (p->mode == MODE_MODIFY) {
                // irrelevant to whether the file still exists or not;
                continue;
            }
            // must be p->mode == MODE_MODIFY here
            if (index_in_filename_list == -1) {
                filenames_in_history[num_of_files_in_history] = calloc(strlen(p->filepath) + 1, sizeof(char));
                strcpy(filenames_in_history[num_of_files_in_history], p->filepath);
                does_file_still_exist_in_dit_tree[num_of_files_in_history] = 1;
                printf("Marking file |%s| as existing (val = %d)...\n", filenames_in_history[num_of_files_in_history], does_file_still_exist_in_dit_tree[num_of_files_in_history]);
                num_of_files_in_history = num_of_files_in_history + 1;
            }
        }
        closedir(commit_dir);
    }

    return num_of_files_in_history;
}