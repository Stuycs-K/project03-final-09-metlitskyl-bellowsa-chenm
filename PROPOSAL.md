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

# Technical Details:

## FILE TRANSFER (Abel)
- socket based file transfer
- forking server accepts requests
- struct transmits file structure and bytes of file
- NULL struct ends transmission 
- OR tar ball

- server adds/overwrites previous files

## TRACKING DIFF (Lenny)

## CONSTRUCTING INTO FINAL FILE (Mathew)
A description of your technical design. This should include:
   
How you will be using the topics covered in class in the project.
     
How you are breaking down the project and who is responsible for which parts.
  
What data structures you will be using and how.
     
What algorithms and /or data structures you will be using, and how.
    
# Intended pacing:

A timeline with expected completion dates of parts of the project.
