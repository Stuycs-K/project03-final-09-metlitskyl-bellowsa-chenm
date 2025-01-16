#include "diff.h" // todo, make h file for matthew
#include "patch.h"
#include "utils.h"
#include <stdlib.h>

char *build_str(int max_commit_number, char *commit_folder, char *filename);
int main(int argc, char *argv[]) {
    char tracked_dir[] = "dit_test_dir/";

    char dit_folder[MAX_FILEPATH] = "";
    strcat(dit_folder, tracked_dir);
    strcat(dit_folder, ".dit/");

    char filename[] = "test.txt";
    char filepath[MAX_FILEPATH] = "";
    strcat(filepath, tracked_dir);
    strcat(filepath, filename);

    char commit_folder[MAX_FILEPATH] = "";
    strcat(commit_folder, dit_folder);
    strcat(commit_folder, "commits/");

    char staging_folder[MAX_FILEPATH] = "";
    strcat(staging_folder, dit_folder);
    strcat(staging_folder, "staging/");

    printf("Filepath : |%s|\n", filepath);
    printf("Corresponding dit folder: |%s|\n", dit_folder);
    printf("Corresponding dit commit folder: |%s|\n", commit_folder);

    // 2. go through git tree commits folder

    // get latest (highest #) commit
    DIR *d;
    d = opendir(commit_folder);
    if (d == 0) {
        err();
    }

    int max_commit_number = -1;

    struct dirent *entry = NULL;
    printf("Directories: \n");
    while ((entry = readdir(d))) {
        if (entry->d_type != DT_DIR) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // char *full_path = join_path(folder_path_with_slash, entry->d_name);
        // total += my_ls(full_path);
        int commit_number = -1;
        sscanf(entry->d_name, "%d", &commit_number);

        printf("commit number (%d) found\n", commit_number);

        if (commit_number > max_commit_number) {
            max_commit_number = commit_number;
        }
    }
    int has_file_been_created_yet = 0; // false
    if (max_commit_number == -1) {
        has_file_been_created_yet = 0;
        printf("NO COMMITS MADE YET!\n");
        printf("Therefore, making a patch commit\n");
    } else {
        printf("Latest commit is: %d\n", max_commit_number);

        for (int i = 0; i <= max_commit_number; i++) {
            char specific_commit_folder[MAX_FILEPATH] = "";

            char folder_name_str[50] = "";
            sprintf(folder_name_str, "%d", i);

            strcat(specific_commit_folder, commit_folder);
            strcat(specific_commit_folder, folder_name_str);

            printf("Specific commit folder: |%s|\n", specific_commit_folder);
            // open dir and apply every patch
            DIR *commit_dir;
            commit_dir = opendir(specific_commit_folder);
            if (commit_dir == 0) {
                err();
            }
            struct dirent *diff_entry = NULL;
            printf("Directories: \n");
            while ((diff_entry = readdir(commit_dir))) {
                if (diff_entry->d_type != DT_REG) {
                    continue;
                }
                if (strcmp(diff_entry->d_name, ".") == 0 || strcmp(diff_entry->d_name, "..") == 0) {
                    continue;
                }

                char patch_full_path[MAX_FILEPATH] = "";
                sprintf(patch_full_path, "%s/%s", specific_commit_folder, diff_entry->d_name);
                printf("Patch found in |%s|\n", patch_full_path);

                // patch name doesn't neccesarily have to be the file name (in case dups/nested)
                Patch *p = read_patch(patch_full_path);

                if (strcmp(p->filepath, filename) != 0) { // just compare filenames, not paths to it
                    continue;
                }
                printf("THIS PATCH MATCHES MY AFFECTED FILE!\n");
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
        printf("\nNeed to build current file in memory!\n");

        char *built = build_str(max_commit_number, commit_folder, filename);

        printf("Built str!\n");
        printf("STR: |%s|\n", built);

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
        printf("FILE WITH USER CHANGES: |%s|\n", file_str);

        // NOW COMPARE
        Patch *p_diff = diff(built, file_str, strlen(built), strlen(file_str));
        strcpy(p_diff->filepath, filename);

        // are there changes?
        if (p_diff->memory_size == 0) {
            printf("THERE ARE NO CHANGES! NOTHING TO ADD...\n");
            return 0;
        }
        printf("THERE ARE CHANGES!\n");

        char patch_name[MAX_FILEPATH] = "";
        strcat(patch_name, filename);
        strcat(patch_name, ".patch");

        char save_patch_to_saving_folder_path[MAX_FILEPATH] = "";
        strcat(save_patch_to_saving_folder_path, staging_folder);
        strcat(save_patch_to_saving_folder_path, patch_name);
        write_patch(save_patch_to_saving_folder_path, p_diff);

        printf("Wrote patch to : |%s|\n", save_patch_to_saving_folder_path);

        // debug and verify
        printf("matthew's patch filename: |%s|\n", p_diff->filepath);
        visualize_patch(p_diff);

        Patch *verify_patch = read_patch(save_patch_to_saving_folder_path);

        size_t new_size;
        char *applied_patch = apply_patch(built, strlen(built), verify_patch, &new_size);

        printf("after applying patch: |%s|\n", applied_patch);
    }
}

char *build_str(int max_commit_number, char *commit_folder, char *filename) { // regular filename NOT filepath
    char *str = calloc(1, sizeof(char));
    for (int i = 0; i <= max_commit_number; i++) {
        char specific_commit_folder[MAX_FILEPATH] = "";

        char folder_name_str[50] = "";
        sprintf(folder_name_str, "%d", i);

        strcat(specific_commit_folder, commit_folder);
        strcat(specific_commit_folder, folder_name_str);

        printf("Specific commit folder: |%s|\n", specific_commit_folder);
        // open dir and apply every patch
        DIR *commit_dir;
        commit_dir = opendir(specific_commit_folder);
        if (commit_dir == 0) {
            err();
        }
        struct dirent *diff_entry = NULL;
        printf("Directories: \n");
        while ((diff_entry = readdir(commit_dir))) {
            if (diff_entry->d_type != DT_REG) {
                continue;
            }
            if (strcmp(diff_entry->d_name, ".") == 0 || strcmp(diff_entry->d_name, "..") == 0) {
                continue;
            }

            char patch_full_path[MAX_FILEPATH] = "";
            sprintf(patch_full_path, "%s/%s", specific_commit_folder, diff_entry->d_name);
            printf("Patch found in |%s|\n", patch_full_path);

            // patch name doesn't neccesarily have to be the file name (in case dups/nested)
            Patch *p = read_patch(patch_full_path);

            if (strcmp(p->filepath, filename) == 0) { // just compare filenames, not paths to it
                printf("THIS PATCH MATCHES MY AFFECTED FILE! need to apply in mem\n");
                // apply patch STRING version (no files modified!)
                if (p->mode == MODE_MODIFY) { //
                                              // apply patch BUT only do it to string in memory (not the file)
                                              // call matthew func and apply!
                    printf("About to apply diff!\n ");
                    size_t new_size;
                    char *applied_patch = apply_patch(str, strlen(str), p, &new_size);

                    printf("after applying patch: |%s|\n", applied_patch);

                    free(applied_patch);
                }
                if (p->mode == MODE_TOUCH) {
                    free(str);
                    str = calloc(p->memory_size + 1, sizeof(char));
                    memmove(str, p->pts, p->memory_size);
                }
                if (p->mode == MODE_REMOVE) {
                    free(str);
                    str = calloc(1, sizeof(char)); // this is byte array not str so not str funcs
                }
            }
            free(p);
        }
        closedir(commit_dir);
    }
    return str;
}