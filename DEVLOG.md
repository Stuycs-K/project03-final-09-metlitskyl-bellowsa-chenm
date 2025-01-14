# Dev Log:

This document must be updated daily by EACH group member.

## Abel

### 2024-01-06 - Established DEVLOG and added basic outline to PROPOSAL

DEVLOG.md file format established
PROPOSAL updated to reflect class discussion
first commits made

### 2024-01-07 - makefile forking server client filesetup

setup files
built makefile
researched to find example code for sockets + examined linux documentation to understand
used lab15 logic + example code + man pages to build functional forking server for sockets
created branch abel -- slightly late... :)
total: abt 1.5hr

### 2024-01-08 - K sample code. tree_transmit
made functional tree_transmit function that trees through a directory and 
transmits name and type of file to server/client.

total: abt 1.5hr


### 2024-01-09 - attempted to get transmit_file fxn to send file contents and not break the system
Reorganized my functions into a file_transfer c file to improve organization

attempted to debug why the client cannot recieve the struct following a file contents transmission...

total: abt 1.5hr

### 2024-01-10 - attempted to get transmit_file fxn to send file contents and not break the system
debugging...

### 2024-01-11 - WORKING SEND/RECV FILES + FIXED CHILD ISSUE + ORGANIZATION + TWO WAY FUNCTIONALITY
Solved problem with the client getting messed up on read. Issue was is was reading too many bytes -> struct was interupted.

improved h file orgination.

"TWO WAY DOWNLOAD?PUSH FROM SERVER NOW WORKSgit add .git add . this is very exciting. to push simply type `./client push <what to push>` to download do `./client download <where to download too>`

time: 2hr:30min

### 2024-01-12 - user struct + server repo system + create usr
created user struct as part of init transmission

now you can push a specific dir and if you have already inited the repo it will work 

pull/push now works. use cmd ./client download/push name_of_repo. will create file/repo if does not exist.

now a new user is created on server when client configs a usr

time: 2.5hr

### 2024-01-13 - push/download from inside rep
pushes and pulls .dit hidden dir inside the repo
stores on server
client init

## Lenny

### 2024-01-06 - Discussion and choosing of project theme, structure with @Abel and @Matthew

abt 20m

### 2024-01-07 - Choose alg; Discuss and design .dit folder

Experiment with different diff algs and their pros/cons
settle on Myers alg for diff alg
Research and experiment with different .dits
abt 40m

### 2024-01-08 - Scaffold out patch

Define what a patch struct is with @matthew
Fiddle with flexible array member to make dynamic memory PART of struct itself
Allocate and create patches on the heap, including support for the unknown memory "trunk" to be part
Write patches to disk by dynamically determining their size
Reading patches from file to buffer struct on heap
abt 1.5h

### 2024-01-09 - Continue basic patch implementation & Assist Abel with debugging

Begin the implementation of modify patch's insertion alg
abt 30m

### 2024-01-10 (IN CLASS) - NO COMMITS, BUT DEBUG WITH ABEL

### 2024-01-12 - Implement modify patch insertion algorithim, implement touch and removal type patches as well

modify patch reads the patches specified file into buff, runs insertion alg on buff, then writes
touch patch creates a file, errors if file alr exists
rm patch removes a file, errors if does not exist
TODO: Modify patch DELETIONS need to also be implemented. Same logic/test case
abt 1h

## Matthew

### 2024-01-06 - Planning

Planned project and decided on its structure with @Abel and @Lenny
40m during class

### 2024-01-07 - Exploration!

Fiddled around with diff command and explored .git folder for inspiration
20m

### 2024-01-08 - Patch planning

Worked with @Lenny to figure out how to format a patch struct and its members
Researched the Myer's diff algorithm from its original paper
1h

### 2024-01-09 - Prelim Diff Draft
Wrote a preliminary diff draft according to its original impl on paper
Still working on tracking the edits
1h

### 2024-01-10 - Refining Diff
Using debug statements to figure out how to backtrack the changes to file
40min

## EXAMPLE

### 2024-01-02 - Brief description

Expanded description including how much time was spent on task.

### 2024-01-03 - Brief description

Expanded description including how much time was spent on task.
