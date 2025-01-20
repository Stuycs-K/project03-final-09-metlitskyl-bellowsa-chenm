#ifndef file_transfer_h
#define file_transfer_h

#include <dirent.h>
#include "user.h"

//file transfer constants
#define TR_FILE 1
#define TR_DIR 2
#define TR_END 3

//file transfer struct sent before a file goes through
struct file_transfer{
    int mode;
    int size;
    char path[1024];
};

// init connection modes
#define TR_RECV 1034
#define TR_TRSMT 5678
#define TR_AINIT 7994
#define TR_RINIT 6689

#define TR_FAIL 45
#define TR_SUCCESS 56

// init connection struct
struct ft_init{
    int mode;
    struct ft_user user;
    char repo_name[1024];
};

//create a new ft_init struct
int new_ft_init(int mode, char * path, struct ft_user * user_in, struct ft_init * init);

// sends the full transmition from init of root to end signal
int send_full_directory_contents(int transmit_fd, char * path);

// recieves the full transmition from init of root to end signal
int recv_full_directory_contents(int recv_fd, char * path);

//sends the end of transmission struct to new_socket
int send_end(int new_socket);

//creates a new file_transfer struct based on the path to the directory the file is in
//and the file name, the dirent entry
int new_file_transfer(char * path, struct dirent * entry, struct file_transfer * ft);

// sends object at path to transmit_fd.
// includes managment of type ie. sends directories and also files
int transmit_file(int transmit_fd, char * path, struct dirent * entry);

// recieves an object
// includes managment of type ie. sends directories and also files
int recv_file(int recv_fd, struct file_transfer * ft); 

// goes through a directory and transmits all files using transmit_file
int tree_transmit(char * path,int transmit_fd);

#endif