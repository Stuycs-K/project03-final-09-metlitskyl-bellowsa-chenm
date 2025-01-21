# Final Project Proposal

## Group Members:
Lenny, Abel, Matthew       

# Intentions:

Implementation of git:
- use diff style method to track changes from a base of an empty string.
- Recurisvely track and find file names and track them for changes from a store cache in .diy
- Sockets with a server to up and down checkpoints/changes/commits/whatever. This will use a custom file transfer protocol we create and be socket based.
    
# Intended usage:

This project will behave very similarly to git-hub. DIT will be used to store work remotly, track changes, and push work to be acceced remotly.

`dit add`: adds files to track list + generate diff/patch -- [lenny]
 - possible extension `dit status`: scans for diffs in all previously tracked files -- [Mathew]
`dit commit`: move all open diffs to a commit_n folder -- [lenny]

`dit push`: push .dit folder to server -- [Abel]
`dit download`: downloads .dit folder from server -- [Abel]

`dit build`: build diffs from start -- Mathew

`dit clone`


# Technical Details:

## FILE TRANSFER (Abel)
- socket based file transfer
- forking server accepts requests
- struct transmits file structure and bytes of file
- NULL struct ends transmission 
- OR tar ball

- server adds/overwrites previous files

## CONSTRUCTING INTO FINAL FILE(Lenny)


## TRACKING DIFF, REVERT (Mathew)
- use the Myer's diff algorithm to find differences and modification in files
- reconstruct strings using the resulting patch of the diff algorithm
- implement the revert feature that reverts files back to a specific commit

A description of your technical design. This should include:
   
How you will be using the topics covered in class in the project.
     
How you are breaking down the project and who is responsible for which parts.
  
What data structures you will be using and how.
     
What algorithms and /or data structures you will be using, and how.
    
# Intended pacing:

A timeline with expected completion dates of parts of the project.
