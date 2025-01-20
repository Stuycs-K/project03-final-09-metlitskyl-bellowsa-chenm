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
    Point pts[];
} Patch;

// yes i know this is defined in dirent.h it is just my intellisense is stupid
#ifndef DT_REG
#define DT_REG 8
#endif

#ifndef DT_DIR
#define DT_DIR 4
#endif

void populate_dit_folders(char *tracked_dir, char *dit_folder, char *commit_folder, char *staging_folder);

int get_max_commit_number(char *tracked_dir);


// STRUCT client_session
// path_to_programdir: max length 1024char path to where dit is located
// int clinet_fd: fd to talk to server
// struct ft_init init: init connection struct
// struct ft_user: user profile
// char repo_name: name of repo
// char repo_name_dit: path to .dit
// char repo_target: ? 
struct client_session{
    char path_to_programdir[1024];
    int client_fd;;
    struct ft_init init;
    struct ft_user user;

    char repo_name[512];
    char repo_name_dit[512];
    char repo_target[1050];
};

void new_client_session(char ** argv, struct client_session * cs);

// file Node recursive directory system
typedef struct file_node {
    char name[1024];
    struct file_node * next;
} FileNode;

FileNode * new_file_node(char * name, FileNode * next);
void free_file_node_list(FileNode * root);
FileNode * get_all_in_dir(char * dir_path, FileNode * root);
void print_file_list(FileNode * root);


//depricated system
#define MAX_FILES 50000

int find_index_in_filename_list(char **filename_list, int num_of_files_in_history, char *search);

int is_directory(char *path);

#endif
