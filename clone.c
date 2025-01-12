#include <sys/stat.h>

int main(){
    mkdir("test_dir2", 0744);
    mkdir("test_dir2/test2", 0744);
}