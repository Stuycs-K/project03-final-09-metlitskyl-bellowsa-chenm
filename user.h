#ifndef user_h
#define user_h


struct ft_user{
    char name[1024];
};

int new_ft_user(char * name, struct ft_user * user);


#endif