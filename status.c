#include "diff.h"
#include "patch.h"
#include "utils.h"
#include "status.h"
#include "build.h"

void status(char *tracked_dir){
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

        char full_path_to_file[MAX_FILEPATH] = "";
        strcat(full_path_to_file, tracked_dir);
        strcat(full_path_to_file, filenames_in_history[i]); 

        if (access(full_path_to_file, F_OK) != 0) {
            printf("File |%s| has been removed.\n", full_path_to_file);
            continue;
        }

        // now see if mod patch needed
        // printf("Abt to build string...\n");
        char *built = build_str(max_commit_number, commit_folder, filenames_in_history[i]);

        // printf("\nLast version of file built up from .dit: |%s|\n\n", built);

        int current_file_with_users_changes = open(full_path_to_file, O_RDONLY);
        if (current_file_with_users_changes == -1) {
            err();
        }

        struct stat file_stat;
        int stat_status = stat(full_path_to_file, &file_stat);
        if (stat_status == -1) {
            err();
        }

        char *file_str = calloc(1, file_stat.st_size + 1);

        int read_status = read(current_file_with_users_changes, file_str, file_stat.st_size);
        if (read_status == -1) {
            err();
        }
        close(current_file_with_users_changes);
        // printf("Current file on disk: |%s|\n\n", file_str);

        // NOW COMPARE
        Patch *p_diff = diff(built, file_str, strlen(built), strlen(file_str));
        strcpy(p_diff->filepath, filenames_in_history[i]);

        // are there changes?
        if (p_diff->memory_size == 0) {
            // printf("THERE ARE NO CHANGES! NOTHING TO ADD...\n");
            continue;
        }
        printf("File |%s| has been modified.\n", full_path_to_file);
    }

    FileNode * root = NULL;
    root = get_all_in_dir(tracked_dir, root);

    for(FileNode * f = root; f; f=f->next){
        char *proper_filename = calloc(strlen(f->name) + 1, sizeof(char));
        strcpy(proper_filename, f->name + 3 );

        // printf("proper filename: |%s|\n", proper_filename);

        int try_to_find_index_in_history = find_index_in_filename_list(filenames_in_history, num_of_files_in_history, proper_filename);
        if (try_to_find_index_in_history > -1 && does_file_still_exist_in_dit_tree[try_to_find_index_in_history]){
            // file DOES exist in git tree
            // printf("File |%s| DOES exist in git tree... ignoring...\n", proper_filename);
            continue;
        }
        printf("File |%s%s| has been added.\n", tracked_dir, proper_filename);

    }
}

int get_files_in_tree(int max_commit_number, char *commit_folder, char **filenames_in_history, int *does_file_still_exist_in_dit_tree){
    int num_of_files_in_history = 0;

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
            strcat(patch_full_path, specific_commit_folder);
            strcat(patch_full_path, "/");
            strcat(patch_full_path, diff_entry->d_name);

            // patch name doesn't neccesarily have to be the file name (in case dups/nested)
            Patch *p = read_patch(patch_full_path);

            // printf("Read patch |%s|!\n", patch_full_path);

            // p-> filepath is just the name of the file inside the tracked dir alr
            int index_in_filename_list = find_index_in_filename_list(filenames_in_history, num_of_files_in_history, p->filepath);
            if (p->mode == MODE_MODIFY) {
                // irrelevant to whether the file still exists or not;
                continue;
            }

            int index_to_mod = index_in_filename_list;

            if (index_in_filename_list == -1) {
                filenames_in_history[num_of_files_in_history] = calloc(strlen(p->filepath) + 1, sizeof(char));
                strcpy(filenames_in_history[num_of_files_in_history], p->filepath);
                index_to_mod = num_of_files_in_history;
                num_of_files_in_history++;
            }

            if (p->mode == MODE_REMOVE) {
                does_file_still_exist_in_dit_tree[index_to_mod] = 0;
                continue;
            }

            if (p->mode == MODE_TOUCH) {
                does_file_still_exist_in_dit_tree[index_to_mod] = 1;
                continue;
            }
        }
        closedir(commit_dir);
    }

    // printf("Visualizing get_files_in_tree...\n");
    // for (int i = 0; i < MAX_FILES; i++){
    //     printf("(%s, %d) ", filenames_in_history[i], does_file_still_exist_in_dit_tree[i]);
        
    // }
    // printf("\n");

    return num_of_files_in_history;
}