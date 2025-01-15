#include "patch.h"
#include "utils.h"
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
        has_file_been_created_yet = 1;
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

                if (strcmp(p->filepath, filename) == 0) { // just compare filenames, not paths to it
                    printf("THIS PATCH MATCHES MY AFFECTED FILE!\n");
                    if (p->mode == MODE_TOUCH) {
                        has_file_been_created_yet = 1;
                    } else if (p->mode == MODE_REMOVE) {
                        has_file_been_created_yet = 0;
                    }
                }
            }
            closedir(commit_dir);
        }
    }

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

        Patch *mypatch = create_patch(filename, MODE_TOUCH, strlen(str), str); // do not do strlen() + 1 bc we want to exclude null byte
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

        char *built_str = calloc(102410240, sizeof(char));

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
                    // apply patch to file
                    if (p->mode == MODE_MODIFY) { //
                        // apply patch BUT only do it to string in memory (not the file)
                    }
                }
            }
            closedir(commit_dir);
        }

        // build current file str in memory (get patches from 0 to max_commit_number, apply patches that match file name to str. Then read the file to a different str and geneerate new diff.)
    }

    // 3. see if file exists
    // 4. if so, send to diff code and make mod patch (store in staging dir)
    // 5. if not, make a simple touch commit
}