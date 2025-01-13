#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//socket stuff
#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>

//signal stuff
#include <signal.h>

//reap children
#include <sys/wait.h>

#include "utils.h"
#include "file_transfer.h"

#define PORT 8080

#include "networking.h"

#define TARGET_DIR "./test_dir"

#define SERVER_DATA "./server_data"

void sighandler(int signo){
  switch(signo){ //child 
    case SIGCHLD:
      {
        while(waitpid(-1,NULL,WNOHANG) > 0);
      }
    // cleanup();
  }

}

int server_action(int new_socket){
  struct ft_init init;
  read(new_socket, &init, sizeof(struct ft_init));
  
  char path[2062];
  switch (init.mode){
    case TR_AINIT:
      printf("RECIVED MAKE USER ORDER\n------------------------------------\n\n");
      sprintf(path, "%s/%s", SERVER_DATA, init.user.name);
      mkdir(path, 0744);
      server_action(new_socket);
      return 0;
      
    case TR_TRSMT:
      printf("RECIVED TRANSMIT ORDER\n------------------------------------\n\n");

      // get_repo_path(SERVER_DATA, &init, path);
      sprintf(path, "%s/%s/%s/", SERVER_DATA, init.user.name, init.repo_name);
      int r = chdir(path);
      v_err(r, "chdir err", 1);
      printf("serving download from %s\n", path);

      
      send_full_directory_contents(new_socket, ".dit");
      break;

    case TR_RECV:
      printf("RECIEVED RECIEVE REQUEST\n------------------------------------\n\n");
      sprintf(path, "%s/%s/%s/", SERVER_DATA, init.user.name, init.repo_name);

      printf("recieving push to %s\n", path);
      recv_full_directory_contents(new_socket, path);
      break;
  }
}

int main(int argc, char const* argv[]){
    signal(SIGCHLD, sighandler); //set SIGCHILD to reaper...

    //set up server listening ...
    int server_fd = setup_server();
    
    //server loop
    while(1){ 
        //main server loop
    
        printf("establishing connection to client...\n");
        
        int new_socket = accept(server_fd, NULL,NULL); //block until a client tries to connect
        
        printf("client connected. forking...\n");
        if(fork()==0){//if fork is child
            // do what the server should do
            server_action(new_socket); 
            close(new_socket);
            printf("closing connection to client...\n");
            exit(0);
        }

        //if we are not the subserver, close the socket to the client
        close(new_socket);
  }

  return 0;
}
