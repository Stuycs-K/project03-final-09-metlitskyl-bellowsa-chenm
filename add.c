#include "build.h"
#include "diff.h" // todo, make h file for matthew
#include "patch.h"
#include "utils.h"
#include <stdlib.h>

void add(char *tracked_dir, char *filename) {
    char filepath[MAX_FILEPATH] = "";
    strcat(filepath, tracked_dir);
    strcat(filepath, filename);

    if (access(filepath, F_OK) != 0) {
        printf("File at |%s| does not exist.\n", filepath);
        exit(1);
    }

    char dit_folder[MAX_FILEPATH] = "";
    char commit_folder[MAX_FILEPATH] = "";
    char staging_folder[MAX_FILEPATH] = "";
    populate_dit_folders(tracked_dir, dit_folder, commit_folder, staging_folder);

    int max_commit_number = get_max_commit_number(tracked_dir);

    int has_file_been_created_yet = 0; // false
    if (max_commit_number == -1) {
        has_file_been_created_yet = 0;
        printf("NO COMMITS MADE YET!\n");
        printf("Therefore, making a patch commit\n");
    } else {
        printf("Latest commit is: %d\n", max_commit_number);

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

    printf("HAS FILE BEEN CREATED YET: |%d|\n", has_file_been_created_yet);

    if (has_file_been_created_yet == 0) {
        // make patch commit!
        // therefore, this must be a patch commit
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
        printf("Read file to buffer str: |%s|\n", str);
        close(in_file);

        Patch *mypatch = create_patch(filename, MODE_TOUCH, strlen(str), (Point *)str); // do not do strlen() + 1 bc we want to exclude null byte
        visualize_patch(mypatch);

        //
        char patch_path[MAX_FILEPATH] = "";
        strcat(patch_path, staging_folder);
        strcat(patch_path, filename);
        strcat(patch_path, ".patch");

        printf("PATCH PATH: |%s|\n", patch_path);
        write_patch(patch_path, mypatch);
    } else {
        printf("Modify patch: Need to build current file in memory!\n");

        char *built = build_str(max_commit_number, commit_folder, filename);

        printf("\nLast version of file built up from .dit: |%s|\n\n", built);

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
        printf("Current file on disk: |%s|\n\n", file_str);

        // NOW COMPARE
        Patch *p_diff = diff(built, file_str, strlen(built), strlen(file_str));
        strcpy(p_diff->filepath, filename);

        // are there changes?
        if (p_diff->memory_size == 0) {
            printf("THERE ARE NO CHANGES! NOTHING TO ADD...\n");
            return;
        }
        printf("THERE ARE CHANGES!\n");

        char patch_name[MAX_FILEPATH] = "";
        strcat(patch_name, filename);
        strcat(patch_name, ".patch");

        char save_patch_to_saving_folder_path[MAX_FILEPATH] = "";
        strcat(save_patch_to_saving_folder_path, staging_folder);
        strcat(save_patch_to_saving_folder_path, patch_name);
        // printf("Trying to write patch to |%s|...\n", save_patch_to_saving_folder_path);
        write_patch(save_patch_to_saving_folder_path, p_diff);

        printf("Wrote patch to : |%s|\n", save_patch_to_saving_folder_path);

        // debug and verify
        // visualize_patch(p_diff);
        // Patch *verify_patch = read_patch(save_patch_to_saving_folder_path);
        // size_t new_size;
        // char *applied_patch = apply_patch(built, strlen(built), verify_patch, &new_size);
        // printf("after applying patch: |%s|\n", applied_patch);
    }
}

int main(int argc, char *argv[]) {
    char tracked_dir[] = "dit_test_dir/";

    if (argc < 2) {
        printf("You must specify the file you want to dit add.\n");
        exit(1);
    }

    add(tracked_dir, argv[1]);
    return 0;
}
