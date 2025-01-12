#ifndef file_transfer_h
#define file_transfer_h

#include <dirent.h>

#define TR_FILE 1
#define TR_DIR 2
#define TR_END 3

struct file_transfer{
    int mode;
    int size;
    char path[1024];
};

// sends the full transmition from init of root to end signal
int send_full_directory_contents(int transmit_fd, char * path);

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