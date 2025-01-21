# Final Project Proposal

## Group Members:

Lenny, Abel, Matthew

# Intentions:

Implementation of git:

-   use diff style method to track changes from a base of an empty string.
-   Recurisvely track and find file names and track them for changes from a store cache in .diy
-   Sockets with a server to up and down checkpoints/changes/commits/whatever. This will use a custom file transfer protocol we create and be socket based.

# Intended usage:

This project will behave very similarly to git-hub. DIT will be used to store work remotly, track changes, and push work to be acceced remotly.

`dit add`: adds files to track list + generate diff/patch -- [lenny]

-   possible extension `dit status`: scans for diffs in all previously tracked files -- [Mathew]
    `dit commit`: move all open diffs to a commit_n folder -- [lenny]

`dit push`: push .dit folder to server -- [Abel]
`dit download`: downloads .dit folder from server -- [Abel]

`dit build`: build diffs from start -- Mathew

`dit clone`

# Technical Details:

## FILE TRANSFER (Abel)

-   socket based file transfer
-   forking server accepts requests
-   struct transmits file structure and bytes of file
-   NULL struct ends transmission
-   OR tar ball

-   server adds/overwrites previous files

## VERSION CONTROL (Lenny)

-   compare file on disk to built file to see if diff; stage and commit diff in modification patch if exists;

-   scan disk for newly created or removed files; create touch/remove patch accordingly

-   create (new, incremented) commit folder from staging dir and make a new clean staging dir

-   build all files in a dir from all the patches within previous commits

-   when building, make sure to create any sub dirs that the files may have resided in too!

-   run all scans for all possible types of changes on `dit status` or `dit add FOLDER_NAME`

## TRACKING DIFF, REVERT (Matthew)

-   use the Myer's diff algorithm to find differences and modification in files

-   reconstruct strings using the resulting patch of the diff algorithm
-   implement the `revert` feature that reverts files back to a specific commit

# Intended pacing:

A timeline with expected completion dates of parts of the project.

Organize specific commands and decide who does what 1/07

Complete file transfer 1/15

Complete diff algo 1/13

Complete outline for patches, then add/commit 1/13

Glue every component together 1/17

Refine everything and fix/report any lasting bugs 1/20
