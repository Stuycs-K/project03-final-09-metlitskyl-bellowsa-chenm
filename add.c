#include "utils.h"

int main(int argc, char *argv[]) {
    char tracked_dir[] = "dit_test_dir/";

    char dit_folder[MAX_FILEPATH];
    strcat(dit_folder, tracked_dir);
    strcat(dit_folder, ".dit/");

    char filename[] = "hi.txt";
    char filepath[MAX_FILEPATH];
    strcat(filepath, tracked_dir);
    strcat(filepath, filename);

    char commit_folder[MAX_FILEPATH];
    strcat(commit_folder, dit_folder);
    strcat(commit_folder, "commits/");

    printf("Filename : |%s|\n", filepath);
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

    if (max_commit_number == -1) {
        printf("NO COMMITS MADE YET!\n");
    } else {
        printf("Latest commit is: %d\n", max_commit_number);
    }

    // 3. see if file exists
    // 4. if so, send to diff code and make mod patch (store in staging dir)
    // 5. if not, make a simple touch commit
}