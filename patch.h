#include "utils.h"

#ifndef PATCH_H
#define PATCH_H
Patch *create_patch(char *filepath, int mode, size_t memory_size, Point *memory);

void visualize_patch(Patch *patch);

void write_patch(char *filename, Patch *patch);

Patch *read_patch(char *filename);

// void apply_modify_patch(Patch *patch);

void apply_touch_patch(Patch *patch);

void apply_delete_patch(Patch *patch);

#endif