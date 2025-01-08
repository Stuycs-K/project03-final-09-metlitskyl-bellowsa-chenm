#include "pipe_networking.h"
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


void sighandler(int signo){
  switch(signo){ //child 
    case SIGINT:
      {
        remove("WKP");
        perror("Exiting due to ctrl-c...");
        exit(0);
      }
    case SIGCHLD:
      {
        printf("reaping child...\n");
        while(waitpid(-1,NULL,0));
      }
    // cleanup();
  }

}

void rando(int * r){
  int fd = open("/dev/random", O_RDONLY, 0);
  read(fd, r, sizeof(int));
  *r = *r % 50 + 50;
  close(fd);
}


void server_cycle(int to_client, int from_client){
  //actual thing it does
  int r;
  rando(&r);
  int bytes;
  while(1){
    bytes = write(to_client, &r, sizeof(r));

    if(bytes != 0 && bytes != -1){
      printf("(%d) sent %d\n",getpid(),r);
      rando(&r);
      sleep(1);
    }
    else{
      break;
    }
    
  }
  printf("client broke connection...\n\n");
}

int main() {
  signal(SIGINT, sighandler);
  signal(SIGCHLD, sighandler);
  while(1){ 
    //main server loop
    printf("establishing connection to client...\n");
    int from_client = server_setup(); //wait for connection...
    printf("recived client...\n");
    printf("forking...\n");
    if(fork()==0){//if fork is child
      int to_client = server_connect(from_client);
      server_cycle(to_client, from_client);
    }
  }
}
