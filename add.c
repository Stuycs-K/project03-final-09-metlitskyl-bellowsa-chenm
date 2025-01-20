#include "build.h"
#include "add.h"
#include "diff.h" // todo, make h file for matthew
#include "patch.h"
#include "utils.h"
#include "status.h"
#include <stdlib.h>

void make_patch_name_safe(char *patchname){
    for (int i =0; i < strlen(patchname); i++){
        if (patchname[i] == '/'){
            patchname[i] = '_';
        }
    }
}

void add_dir(char *tracked_dir, char *filename){
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

        // skip if does not start with file name
        if (strcmp(filename, ".") != 0 && strncmp(filenames_in_history[i], filename, strlen(filename)) != 0){
            continue;
        }

        char full_path_to_file[MAX_FILEPATH] = "";
        strcat(full_path_to_file, tracked_dir);
        strcat(full_path_to_file, filenames_in_history[i]); 

        if (access(full_path_to_file, F_OK) != 0) {
            // stage a removal patch
            add(tracked_dir, filenames_in_history[i], 0);
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
        add(tracked_dir, filenames_in_history[i], 1);
    }

    FileNode * root = NULL;
    root = get_all_in_dir(tracked_dir, root);

    for(FileNode * f = root; f; f=f->next){
        char *proper_filename = calloc(strlen(f->name) + 1, sizeof(char));
        strcpy(proper_filename, f->name + 3 );

        // skip if does not start with file name
        if (strcmp(filename, ".") != 0 && strncmp(proper_filename, filename, strlen(filename)) != 0){
            continue;
        }

        int try_to_find_index_in_history = find_index_in_filename_list(filenames_in_history, num_of_files_in_history, proper_filename);
        if (try_to_find_index_in_history > -1 && does_file_still_exist_in_dit_tree[try_to_find_index_in_history]){
            // file DOES exist in git tree
            // printf("File |%s| DOES exist in git tree... ignoring...\n", proper_filename);
            continue;
        }
        add(tracked_dir, proper_filename, 1);
    }
}

void add(char *tracked_dir, char *filename, int shouldCheckIfDeleted) {
    char filepath[MAX_FILEPATH] = "";
    strcat(filepath, tracked_dir);
    strcat(filepath, filename);
    
    char dit_folder[MAX_FILEPATH] = "";
    char commit_folder[MAX_FILEPATH] = "";
    char staging_folder[MAX_FILEPATH] = "";
    populate_dit_folders(tracked_dir, dit_folder, commit_folder, staging_folder);

    int max_commit_number = get_max_commit_number(tracked_dir);

    if (access(filepath, F_OK) != 0) {
        if (shouldCheckIfDeleted){ // this check always runs UNLESS this came from "add ." which already ran it
            char **filenames_in_history = calloc(MAX_FILES, sizeof(char *));
            int *does_file_still_exist_in_dit_tree = calloc(MAX_FILES, sizeof(int));

            int num_of_files_in_history = get_files_in_tree(max_commit_number, commit_folder, filenames_in_history, does_file_still_exist_in_dit_tree);

            int try_to_find_index_in_history = find_index_in_filename_list(filenames_in_history, num_of_files_in_history, filename);
            if (try_to_find_index_in_history == -1){
                printf("Dit tree AND filesystem both have no recollection of |%s|. We cannot track what does not exist.\n", filepath);
                exit(1);
            }

        }

        Patch* delete_patch = create_patch(filename, MODE_REMOVE, 0, NULL);

        char patch_name[MAX_FILEPATH] = "";
        strcat(patch_name, filename);
        strcat(patch_name, ".patch");
        make_patch_name_safe(patch_name);

        char save_patch_to_saving_folder_path[MAX_FILEPATH] = "";
        strcat(save_patch_to_saving_folder_path, staging_folder);
        strcat(save_patch_to_saving_folder_path, patch_name);
        write_patch(save_patch_to_saving_folder_path, delete_patch);
        printf("File |%s| was deleted, so wrote a remove patch to |%s|\n", filepath, save_patch_to_saving_folder_path);

        return;
    }

    if (is_directory(filepath)){
        add_dir(tracked_dir, filename);
        return;
    }


    int has_file_been_created_yet = 0; // false
    if (max_commit_number == -1) { // if no commits made yet, any "add" of a file must be of mode touch
        has_file_been_created_yet = 0;
    } else {
        // printf("Latest commit is: %d\n", max_commit_number);

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

                if (strcmp(p->filepath, filename) != 0) { // just compare filenames, not paths to it
                    continue;
                }
                if (p->mode == MODE_TOUCH) {
                    has_file_been_created_yet = 1;
                } else if (p->mode == MODE_REMOVE) {
                    has_file_been_created_yet = 0;
                }
            }
            closedir(commit_dir);
        }
    }

    // printf("HAS FILE BEEN CREATED YET IN THE DIT TREE: |%d|\n", has_file_been_created_yet);

    if (has_file_been_created_yet == 0) {
        // make touch patch commit!
        // therefore, this must be a touch patch commit
        // Step 1: Get str from file
        int in_file = open(filepath, O_RDONLY);

        if (in_file == -1) {
            err();
        }

        struct stat file_stat;
        int stat_status = stat(filepath, &file_stat);
        if (stat_status == -1) {
            err();
        }

        size_t file_size = file_stat.st_size;
        char *str = calloc(1, file_size + 1);

        int read_status = read(in_file, str, file_size);
        if (read_status == -1) {
            err();
        }
        // printf("Read disk file to buffer str: |%s|\n", str);
        close(in_file);

        Patch *mypatch = create_patch(filename, MODE_TOUCH, strlen(str), (Point *)str); // do not do strlen() + 1 bc we want to exclude null byte
        
        char patch_name[MAX_FILEPATH] = "";
        strcat(patch_name, filename);
        strcat(patch_name, ".patch");
        make_patch_name_safe(patch_name);
        //
        char patch_path[MAX_FILEPATH] = "";
        strcat(patch_path, staging_folder);
        strcat(patch_path, patch_name);

        // printf("TOUCH PATCH PATH: |%s|\n", patch_path);
        write_patch(patch_path, mypatch);
        printf("File |%s| was added, so wrote a touch patch to |%s|\n", filepath, patch_path);
    } else {
        // printf("Modify patch: Need to build current file in memory!\n");

        char *built = build_str(max_commit_number, commit_folder, filename);

        // printf("\nLast version of file built up from .dit: |%s|\n\n", built);

        int current_file_with_users_changes = open(filepath, O_RDONLY);
        if (current_file_with_users_changes == -1) {
            err();
        }

        struct stat file_stat;
        int stat_status = stat(filepath, &file_stat);
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
        strcpy(p_diff->filepath, filename);

        // are there changes?
        if (p_diff->memory_size == 0) {
            // printf("THERE ARE NO CHANGES! NOTHING TO ADD...\n");
            return;
        }
        // printf("THERE ARE CHANGES!\n");

        char patch_name[MAX_FILEPATH] = "";
        strcat(patch_name, filename);
        strcat(patch_name, ".patch");
        make_patch_name_safe(patch_name);

        char save_patch_to_saving_folder_path[MAX_FILEPATH] = "";
        strcat(save_patch_to_saving_folder_path, staging_folder);
        strcat(save_patch_to_saving_folder_path, patch_name);
        // printf("Trying to write patch to |%s|...\n", save_patch_to_saving_folder_path);
        write_patch(save_patch_to_saving_folder_path, p_diff);

        // printf("Wrote patch to : |%s|\n", save_patch_to_saving_folder_path);
        
        printf("File |%s| was modified, so wrote a modify patch to |%s|\n", filepath, save_patch_to_saving_folder_path);

        // debug and verify
        // visualize_patch(p_diff);
        // Patch *verify_patch = read_patch(save_patch_to_saving_folder_path);
        // size_t new_size;
        // char *applied_patch = apply_patch(built, strlen(built), verify_patch, &new_size);
        // printf("after applying patch: |%s|\n", applied_patch);
    }
}

// int main(int argc, char *argv[]) {
//     char tracked_dir[] = "dit_test_dir/";

//     if (argc < 2) {
//         printf("You must specify the file you want to dit add.\n");
//         exit(1);
//     }

//     add(tracked_dir, argv[1]);
//     return 0;
// }
