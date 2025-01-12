#ifndef user_h
#define user_h


struct ft_user{
    char name[1024];
};

int new_ft_user(char * name, struct ft_user * user);

//create a new config_dir for a client if it does not exist
// read contents of .client_config/user to user struct
int init_client_config(char * program_name, struct ft_user * user);

#endif