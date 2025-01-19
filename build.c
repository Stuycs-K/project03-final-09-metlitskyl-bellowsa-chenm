#include "build.h"
#include "diff.h"
#include "patch.h"
#include "utils.h"
#include "status.h"

char *build_str(int max_commit_number, char *commit_folder, char *filename) { // regular filename NOT filepath
    char *str = calloc(1, sizeof(char));
    for (int i = 0; i <= max_commit_number; i++) {
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

            if (strcmp(p->filepath, filename) == 0) { // just compare filenames, not paths to it
                // printf("THIS PATCH MATCHES MY AFFECTED FILE! need to apply in mem\n");
                // apply patch STRING version (no files modified!)
                if (p->mode == MODE_MODIFY) {
                    size_t new_size;
                    char *applied_patch = apply_modify_patch_to_string(str, strlen(str), p, &new_size);

                    // printf("after applying patch: |%s|\n", applied_patch);

                    free(str);
                    str = applied_patch;
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

int find_index_in_filename_list(char **filename_list, int max_num_files, char *search) {
    for (int i = 0; i < max_num_files; i++) {
        if (strcmp(filename_list[i], search) == 0) {
            return i;
        }
    }
    return -1;
}

void build(char *tracked_dir) {
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
        printf("\nFILE |%s| still exists!\n", filenames_in_history[i]);
        char *built = build_str(max_commit_number, commit_folder, filenames_in_history[i]);
        printf("Built str: |%s|\n", built);

        char full_path_to_file[MAX_FILEPATH] = "";
        strcat(full_path_to_file, tracked_dir);
        strcat(full_path_to_file, filenames_in_history[i]);

        // write built str to file!
        int out_file = open(full_path_to_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (out_file == -1) {
            err();
        }

        int write_status = write(out_file, built, strlen(built));
        if (write_status == -1) {
            err();
        }
        close(out_file);
    }

    // for every file detected:
}

// int main() {
//     build("dit_test_dir/");
// }