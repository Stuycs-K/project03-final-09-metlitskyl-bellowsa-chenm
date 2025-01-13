#include "utils.h"

struct patch *create_patch(char *filepath, int mode, size_t memory_size, char *memory) {
    struct patch *patch = calloc(1, sizeof(struct patch) + memory_size * sizeof(char));

    strcpy(patch->filepath, filepath);
    patch->mode = mode;
    patch->memory_size = memory_size;
    memcpy(patch->memory, memory, memory_size);

    return patch;
}

void visualize_patch(struct patch *patch) {
    printf("----Visualizing Patch: ------\n");
    printf("filepath: |%s|\n", patch->filepath);
    printf("memory size: |%d|\n", patch->memory_size);
    printf("raw memory: |%s|\n", patch->memory);
    printf("----End visualizing. ------\n");
}

void write_patch(char *filename, struct patch *patch) {
    int out_file = open(filename, O_CREAT | O_WRONLY, 0644);
    if (out_file == -1) {
        err();
    }
    size_t struct_byte_size = sizeof(struct patch) + patch->memory_size; // min size is sizeof(struct patch)

    int write_status = write(out_file, patch, struct_byte_size);
    if (write_status == -1) {
        err();
    }
    close(out_file);
}
struct patch *read_patch(char *filename) {
    int in_file = open(filename, O_RDONLY);
    if (in_file == -1) {
        err();
    }

    struct stat file_stat;
    int stat_status = stat(filename, &file_stat);
    if (stat_status == -1) {
        err();
    }

    size_t patch_file_size = file_stat.st_size;
    struct patch *patch = calloc(1, patch_file_size);

    int read_status = read(in_file, patch, patch_file_size);
    if (read_status == -1) {
        err();
    }
    close(in_file);
    return patch;
}

void apply_modify_patch(struct patch *patch) {

    // Step 1: Get str from file
    int in_file = open(patch->filepath, O_RDONLY);

    if (in_file == -1) {
        if (errno == ENOENT) {
            printf("MODIFY PATCH FAILED: FILE |%s| DOES NOT EXIST!!\n", patch->filepath);
            exit(errno);
        }
        err();
    }

    struct stat file_stat;
    int stat_status = stat(patch->filepath, &file_stat);
    if (stat_status == -1) {
        err();
    }

    size_t patch_file_size = file_stat.st_size;
    char *str = calloc(1, patch_file_size + 1);

    int read_status = read(in_file, str, patch_file_size);
    if (read_status == -1) {
        err();
    }
    printf("Read file to buffer str: |%s|\n", str);
    close(in_file);

    // Step 2: Generate new text from patch
    char *head = (patch->memory); // point to start

    while (head < patch->memory + patch->memory_size) { // loop until hits end of memory buffer
        // THIS IS THE ONE CHANGE BLOCK
        // grab char
        char plus_or_minus = *(head);
        head += sizeof(char);

        // grab int
        int location = *(int *)(head);
        head += sizeof(int);

        // grab int
        int number_of_bytes = *(int *)(head);
        head += sizeof(int);

        printf("Plus or Minus: |%c|\n", plus_or_minus);
        printf("Location: |%d|\n", location);
        printf("number_of_bytes: |%d|\n", number_of_bytes);

        if (plus_or_minus == MODE_PLUS) {
            // assume worst case scenario that buffer is exactly size of str
            size_t new_str_len = strlen(str) + number_of_bytes + 1; // +1 for null byte
            str = realloc(str, sizeof(char) * (new_str_len));
            str[new_str_len - 1] = '\0'; // just in case so you don't shoot yourself in the foot

            // shift stuff after our location (including the byte at location) to the right
            int max_number_of_bytes_to_copy = strlen(str) - location;
            strncpy(str + location + number_of_bytes, str + location, max_number_of_bytes_to_copy); // dest, src, # to copy

            // now read bytes
            for (int i = 0; i < number_of_bytes; i++) {
                // grab char
                char new_byte = *(head);

                str[location + i] = new_byte;

                head += sizeof(char); // advance to next byte
            }
        } else if (plus_or_minus == MODE_MINUS) {
            // only a shift left is needed (no need to shrink str)
            // then move null byte back accordingly (strlen in the write will handle this!)
        }
    }

    // Step 3: Write to file!
    int out_file = open(patch->filepath, O_WRONLY | O_TRUNC, 0644);
    if (out_file == -1) {
        err();
    }

    int write_status = write(out_file, str, strlen(str));
    if (write_status == -1) {
        err();
    }
    close(out_file);
}

void apply_touch_patch(struct patch *patch) {
    char *filepath = patch->filepath;
    int fd = open(filepath, O_WRONLY | O_CREAT | O_EXCL, 0644); // O_EXCL is "error if create and file exists"
    if (fd == -1) {
        if (errno == EEXIST) {
            printf("TOUCH PATCH FAILED: FILE |%s| ALREADY EXISTS!!\n", patch->filepath);
            exit(errno);
        }
        err();
    }

    if (patch->memory_size > 0) {
        printf("Trying to write text from patch memory to newly created file...\n");
        // treat memory as JUST a str (char*) NOT a fancy encoded modification patch memory thingy
        int write_status = write(fd, patch->memory, patch->memory_size);
        if (write_status == -1) {
            err();
        }
    }

    close(fd);
}

void apply_delete_patch(struct patch *patch) {
    char *filepath = patch->filepath;
    int removal_status = remove(filepath);
    if (removal_status == -1) {
        err();
    }
}

int main() {
    char txt[] = "hi\nline2\nline3";
    struct patch *mypatch = create_patch("test/hi_write_then_read_and_apply.txt", MODE_TOUCH, strlen(txt), txt); // do not do strlen() + 1 bc we want to exclude null byte
    visualize_patch(mypatch);

    char filename[] = ".dit/patch1.patch";
    printf("Writing patch...\n");
    write_patch(filename, mypatch);
    printf("Patch written!\n");

    struct patch *mypatch2 = read_patch(filename);
    printf("Patch 2 read... visualizing now!\n");
    visualize_patch(mypatch2);
    printf("Writing the patch that we read off disk...\n");
    apply_touch_patch(mypatch2);

    char mem[] = {
        '+', 3, 0, 0, 0, 2, 0, 0, 0, 'n', 'm',
        //   '+', 7, 0, 0, 0, 2, 0, 0, 0, 'x', 'y'
    };
    //  '+', 3, 0, 0, 0, 2, 0, 0, 0, 'n', 'm' is one change
    //  '+', 7, 0, 0, 0, 2, 0, 0, 0, 'x', 'y' is one change
    struct patch *test_patch = create_patch("test/matthew.txt", MODE_MODIFY, sizeof(mem), mem);
    write_patch(".dit/matthew1.patch", test_patch);
    apply_modify_patch(test_patch);

    printf("About to create a file using a touch patch...\n");
    struct patch *test_touch_patch = create_patch("test/hi.txt", MODE_TOUCH, 0, NULL);
    apply_touch_patch(test_touch_patch);
    printf("Created!\n");

    sleep(2);

    printf("About to delete a file using a touch patch...\n");
    struct patch *test_removal_patch = create_patch("test/hi.txt", MODE_REMOVE, 0, NULL);
    apply_delete_patch(test_removal_patch);
    printf("Deleted!\n");

    return 0;
}