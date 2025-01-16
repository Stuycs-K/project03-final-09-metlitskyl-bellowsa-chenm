#include "utils.h"
#ifndef DIFF_H

#define DIFF_H

Patch *diff(const char *a, const char *b, size_t a_length, size_t b_length);

char *apply_patch(char *arr, size_t arr_length, Patch *p, size_t *new_size);

#endif