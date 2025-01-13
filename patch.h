#include "utils.h"

#ifndef PATCH_H
#define PATCH_H
struct patch *create_patch(char *filepath, int mode, size_t memory_size, char *memory);

void visualize_patch(struct patch *patch);

void write_patch(char *filename, struct patch *patch);

struct patch *read_patch(char *filename);

void apply_modify_patch(struct patch *patch);

void apply_touch_patch(struct patch *patch);

void apply_delete_patch(struct patch *patch);

#endif