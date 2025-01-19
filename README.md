[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# DIT: DIY-GIT

### LAMB: L(enny)A(bel)M(atthew)B

Lenny, Abel, Matthew
       
### Project Description:

Implementation of git:

use diff to track changes from a base of an empty string.
Implement [Myers diff algorithims](https://ably.com/blog/practical-guide-to-diff-algorithms) to track changes from a previous version (from a base of an empty string).

Recurisvely track and find file names and track them for changes from a store cache in .diy

Sockets with a server to up and down checkpoints/changes/commits/whatever.

Explain what is this project.
  
### Instructions:

#### setup server

Take note of the public ip address of the server (if you are uncertain of how to do this, run `curl -4 ifconfig.co` which will print the server ip address.) you will later need this ip address to connect to your server from another computer. Ensure that the server and the clinet will be able to connect. On Stuy machines, this means that both the server and the clinet need to be stuy cs lab machines with ip adresses of `149.89.161.1<computer #>`.

Run `make server` to create the server program. Then run `./server` to run the server. This server will act as the remote storage system for the dit repo's that will be created. 

After you have completed the setup server stage, the server should be displaying the message `establishing connection to client... `.

#### use on client

First make the dit program by running `make dit`. When complete, you should see a program called dit. If you wish to alias dit run `alias dit=<PATH TO DIT PROGRAM>`. For the rest of the documentation, it will be assumed that you aliased the path to the dit program as dit. If you did not, replace `dit` with `<PATH TO DIT PROGRAM>` in the following instruction.

To configure your user acount run `dit user`. You will be prompted to enter in your username, music preference and the ip address of the server. A `.client_config` containing a file called `user` will be created in the same directory that `dit` is installed in. If your information or choices change, `dit user` will override the previous user configuration.

To make a directory a dit repo, run `dit init`.

build with make build

run server with ./server

alias dit if you would like



