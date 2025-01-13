#include "file_transfer.h"
#include "user.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <unistd.h>

#ifndef utils_h
#define utils_h

#define EXIT 1
#define NO_EXIT 0
int v_err(int sigerr, char *msg, int _exit);
void err();

#define MAX_FILEPATH 512

#define MODE_TOUCH 0
#define MODE_MODIFY 1 // modifying TEXT inside a file
#define MODE_REMOVE 2

#define MODE_PLUS '+'
#define MODE_MINUS '-'

struct patch {
    char filepath[MAX_FILEPATH];
    int mode;
    size_t memory_size;
    char memory[]; // FLEXIBLE ARRAY MEMBBER, unkown length of memory raw memory, used for chars in mode_touch, used for our diff format in mode_modify, 1 byte for mode_remove
};

int get_repo_path(char *server_root, struct ft_init *init, char *target);

// yes i know this is defined in dirent.h it is just my intellisense is stupid
#ifndef DT_REG
#define DT_REG 8
#endif

#ifndef DT_DIR
#define DT_DIR 4
#endif

#endif