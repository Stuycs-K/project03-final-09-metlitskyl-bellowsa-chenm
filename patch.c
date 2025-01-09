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
}

void apply_patch(char *str, struct patch *patch) {
    int new_size = 9;
    char *head = (patch->memory); // point to start

    while (head < patch->memory + patch->memory_size) { // loop until hits end of memory buffer
        // THIS IS THE ONE CHANGE BLOCK
        // grab char
        char plus_or_minus = *(head + 0);
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

            // now that we have space, insert!
            // save stuff at [location, end]
            // insert at location
        }
    }
}

int main() {
    char txt[] = "hi\nline2\nline3";
    struct patch *mypatch = create_patch("hi.txt", MODE_TOUCH, strlen(txt) + 1, txt);
    visualize_patch(mypatch);

    char filename[] = ".dit/patch1.patch";
    printf("Writing patch...\n");
    write_patch(filename, mypatch);
    printf("Patch written!\n");

    struct patch *mypatch2 = read_patch(filename);
    printf("Patch read!\n");
    visualize_patch(mypatch);

    char mem[] = {'+', 3, 0, 0, 0, 2, 0, 0, 0, 'n', 'm',
                  '+', 7, 0, 0, 0, 2, 0, 0, 0, 'x', 'y'};
    //  '+', 3, 0, 0, 0, 2, 0, 0, 0, 'n', 'm' is one change
    //  '+', 7, 0, 0, 0, 2, 0, 0, 0, 'x', 'y' is one change
    struct patch *test_patch = create_patch("matthew.txt", MODE_MODIFY, sizeof(mem), mem);
    char buffer[512] = "abcdefghijk";
    write_patch(".dit/matthew1.patch", test_patch);
    apply_patch(buffer, test_patch);

    return 0;
}