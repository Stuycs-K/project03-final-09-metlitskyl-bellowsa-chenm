#include "utils.h"

Patch *create_patch(char *filepath, int mode, size_t memory_size, char *memory) {
    Patch *patch = calloc(1, sizeof(Patch) + memory_size * sizeof(char));

    strcpy(patch->filepath, filepath);
    patch->mode = mode;
    patch->memory_size = memory_size;
    memcpy(patch->memory, memory, memory_size);

    return patch;
}

void visualize_patch(Patch *patch) {
    printf("----Visualizing Patch: ------\n");
    printf("filepath: |%s|\n", patch->filepath);
    printf("memory size: |%d|\n", patch->memory_size);
    printf("raw memory: |%s|\n", patch->memory);
    printf("----End visualizing. ------\n");
}

void write_patch(char *filename, Patch *patch) {
    int out_file = open(filename, O_CREAT | O_WRONLY, 0644);
    if (out_file == -1) {
        err();
    }
    size_t struct_byte_size = sizeof(Patch) + patch->memory_size; // min size is sizeof(Patch)

    int write_status = write(out_file, patch, struct_byte_size);
    if (write_status == -1) {
        err();
    }
}
Patch *read_patch(char *filename) {
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
    Patch *patch = calloc(1, patch_file_size);

    int read_status = read(in_file, patch, patch_file_size);
    if (read_status == -1) {
        err();
    }
}

int main() {
    char txt[] = "hi\nline2\nline3";
    Patch *mypatch = create_patch("hi.txt", MODE_TOUCH, strlen(txt) + 1, txt);
    visualize_patch(mypatch);

    char filename[] = ".dit/patch1.patch";
    printf("Writing patch...\n");
    write_patch(filename, mypatch);
    printf("Patch written!\n");

    Patch *mypatch2 = read_patch(filename);
    printf("Patch read!\n");
    visualize_patch(mypatch);

    return 0;
}