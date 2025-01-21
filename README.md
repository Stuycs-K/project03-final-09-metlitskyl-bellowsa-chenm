[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)

# DIT: DIY-GIT

## [VIDEO LINK](https://drive.google.com/file/d/1QtEMatKopxF7L26c0qwVWyi0m8suEGyZ/view?usp=sharing)

### LAMB: L(enny)A(bel)M(atthew)B

Lenny, Abel, Matthew

### Project Description: Implementation of GIT

Use diffs to track changes from a base of an empty string.
Implement [Myers diff algorithims](https://ably.com/blog/practical-guide-to-diff-algorithms) to track changes from a previous version (from a base of an empty string).

Allows user to track file status (has a fille been modified/created/deleted), stage changes, commit changes, revert changes.

Use networking and sockets to recursivly transmit directories from client to server and server to client in pull/push exchanges. Build a full repositiory with just from the changes and commits within a .dit

### Instructions:

#### (Prelimanary) Server Setup

Take note of the public ip address of the server (if you are uncertain of how to do this, run `curl -4 ifconfig.co` which will print the server ip address.) you will later need this ip address to connect to your server from another computer. Ensure that the server and the clinet will be able to connect. On Stuy machines, this means that both the server and the clinet need to be stuy cs lab machines with ip adresses of `149.89.161.1<computer #>`.

Run `make server` to create the server program. Then run `./server` to run the server. This server will act as the remote storage system for the dit repo's that will be created.

After you have completed the setup server stage, the server should be displaying the message `establishing connection to client... `.

#### Client Use

First make the dit program by running `make dit`. When complete, you should see a program called dit. If you wish to alias dit run `alias dit=<PATH TO DIT PROGRAM>`. For the rest of the documentation, it will be assumed that you aliased the path to the dit program as dit. If you did not, replace `dit` with `<PATH TO DIT PROGRAM>` in the following instruction.

To configure your user acount run `dit user`. You will be prompted to enter in your username, music preference and the ip address of the server. A `.client_config` containing a file called `user` will be created in the same directory that `dit` is installed in. If your information or choices change, `dit user` will override the previous user configuration.

To make a directory a dit repo, run `dit init` inside the repo. This will make a repo on the server and create the nececary .dit directory with a commits and staging folder.

To pull from a repo, cd into the repo and then run either `dit pull` or `dit download && dit build`

To push your .dit directory run `dit push`

To see what changes have been made in your .dit directory, run `dit status`

To stage a file for commiting run `dit add <file>` (ie: `dit add test.txt`)

To stage everything within a folder for commiting run `dit add <file>` (ie: `dit add .` or `dit add foo/`)

To commit run `dit commit`

To revert a file to an old commit run `dit revert <commit #> <file>`

-   note that this only works for specific files, not entire directory
-   also note this only works for files that existed in the old commit

### Bugs

-   Ocationally, you may need to run `dit push`, `dit pull`, or `dit init` several times. We are not sure why this occurs and it only occurs when lenny runs the server and Abel runs the client. This error results in failure messages or incomplete file transfers. If you encounter this error, try either picking a different lab machine to use as your server or using a different series of user acounts.
