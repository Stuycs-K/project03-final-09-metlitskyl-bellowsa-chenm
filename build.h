#ifndef BUILD_H

#define BUILD_H

char *build_str(int max_commit_number, char *commit_folder, char *filename);

void build(char *tracked_dir);

void create_missing_dirs_to_place_file(char* tracked_dir, char* rel_filepath);

#endif