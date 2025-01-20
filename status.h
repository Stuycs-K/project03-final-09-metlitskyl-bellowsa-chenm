#ifndef STATUS_H

#define STATUS_H

void status(char *commit_folder);

int find_index_in_filename_list(char **filename_list, int max_num_files, char *search);

int get_files_in_tree(int max_commit_number, char *commit_folder, char **filenames_in_history, int *does_file_still_exist_in_dit_tree);

#endif