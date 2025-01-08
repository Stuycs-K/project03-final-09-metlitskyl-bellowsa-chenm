#ifndef utils_h
#define utils_h

#define EXIT 1
#define NO_EXIT 0;

int v_err(int sigerr, char * msg, int _exit);
int tree_transmit(char * path,int transmit_fd);

#endif