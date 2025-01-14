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

#define MODE_TOUCH 1
#define MODE_MODIFY 2 // modifying TEXT inside a file
#define MODE_REMOVE 3

#define INSERT_TYPE 3
#define DELETE_TYPE 4

/*
Point update
type: INSERT or DELETE
pos: byte location to make edit in file
ch: char to insert, nullchar if DELETE
*/
typedef struct {
    int type;
    int pos;
    char ch;
} Point;

/*
Patch
filepath: relative file path location
mode: MODE_TOUCH (create file), MODE_MODIFY (modify file contents), MODE_REMOVE (remove file)
memory_size: size of pts array, zero if REMOVE
pts: contains point updates, empty if REMOVE
*/
typedef struct {
    char filepath[MAX_FILEPATH];
    int mode;
    size_t memory_size;
    Point *pts;
} Patch;

#endif
