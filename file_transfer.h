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

int send_end(int new_socket);
int new_file_transfer(char * path, struct dirent * entry, struct file_transfer * ft);
int transmit_file(int transmit_fd, char * path, struct dirent * entry);
int recv_file(int recv_fd, struct file_transfer * ft); 
int tree_transmit(char * path,int transmit_fd);

#endif