#include "user.h"
#include <string.h>

int new_ft_user(char * name, struct ft_user * user){
    strcpy(user->name, name);
}