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
time 1.5hr

### 2024-01-14 - play sounds on actions

sound system plays sound effects
time: 30min

### 2024-01-15 - added user customization options to play sound

user customization
commenting
time: 45min

### 2024-01-16 - begining dit merge

dit client works
time: 45min

### 2024-01-17 - merged dit networking with dit version control

with mathew and lenny I merged dit networking with version control ->
now dit can do all needed functions
all left to do id DOCS + refactoring
time: 45min

### 2024-01-19 - refactor and ip

isolated all functions into their own c/h files
refactored ip system and user system to allow server customization
created file_node recursive system
worked on documentation
time: 4hr
### 2024-01-20 - refactor and bug fixes and documentation and video
improved documentation, err handling, strange lingering bugs
finished man pages
built video
solved bugs related to init with add. directories present.
time: 4hr

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

### 2024-01-10 (IN CLASS) - NO COMMITS, BUT DEBUG SERVER IPS WITH ABEL

abt 1h

### 2024-01-12 - Implement modify patch insertion algorithim, implement touch and removal type patches as well

modify patch reads the patches specified file into buff, runs insertion alg on buff, then writes
touch patch creates a file, errors if file alr exists
rm patch removes a file, errors if does not exist
TODO: Modify patch DELETIONS need to also be implemented. Same logic/test case

abt 1h

### 2024-01-13 - Begin dit add & dit build

-   Make touch patches create file with CONTENTS
-   Build structure of .dit folder (/commits/0, /commits/1, /commits/2 etc )
-   When adding (proposing a stage) a file, see if it any previous commits have touchd this file
-   if so, current commit is modification type; begin to build old version of file in memory to be able to compare current version to to gen NEW mod commit
-   if not, generate new touch commit

abt 4h

### 2024-01-14 - Apply matthew's new mod patch alg and refactor to use Point typedef struct

abt 30m

### 2024-01-15 - DEBUGGING - Help debug missing mysterious packets with Abel; Classtime spent with Mr. K debugging execvp lab

abt 1h

### 2024-01-16 - DIT ADD command implemented for all touch and modification; Dit commit;

-   dit add <filename> now builds str in memory and compares it to version on disk to generate a new mod commit
-   dit build can build all files within a .dit history and save them to disk too

abt 3h

### 2024-01-17 - Migrate mains to void funcs from build, commit, and add to glue together into one DIT command

-   glueing involved make hell and fixing circular deps

abt 1h

### 2024-01-19 - Dit STATUS and dit add DOT

-   scan all files in a dit tree for modifications/ deletions
-   scan all files in tracked dir on disk to see if any new ones (not previously in dit tree) have been added
-   dit add dot (or any other filename) can FINALLY make removal type patches and commits by scanning current tree in the same way as dit status

abt 5h

### 2024-01-20 - Final cleanup and bug fixes

-   remove all print statments
-   dit add <removed file> (not just the dir where a removed file is) also works now by taking inspo from tree scanning of dit status
-   merge abel's code that allows nested dirs to be committed
-   update build to create folders as needed when nested dir does not exist yet

abt 4h

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

### 2024-01-11 to 2024-01-12 - Tracing point updates

Figured out how to properly backtrack edits

Reformatted how patches are going to be stored/handled

1h

### 2024-01-13 - Applying point updates

Wrote apply_patch which will apply a modify patch to a string

Did testing to verify it works

1h

### 2024-01-14 to 2024-01-17 - Working w Group

Worked with Lenny to incorporate my diff.c code with his add/commit code

Also fixed a minor bug on my part for diff.c for changes at the front of file

40m per day

### 2024-01-19 - Reading Lenny's code

Trying to understand Lenny's code in order to work on revert

Wrote some of the revert function

1h

### 2024-01-20 - Finished Revert

Completed revert, did some testing as well

1h
