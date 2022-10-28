#include <stdin.h>
#include <unistd.h>

//в аргументах счетчик и указатель на нулевой индекс каждого элемента
int main(int argc, char *argv[]) {
    typedef struct options {
        int b;
        int e;
        int n;
        int s;
        int v;
    } opt;
    
    opt options = {0};
    
     parser(argc, argv, &options)
    return 0;
};

void parser(int argc, *char argv[], opt *options) {
    int opt;
    int option_index;
    static struct option long_options[]
    
    while opt = getopt_long(argc, argv, "benstv", long_options, &option_index) != -1;
    switch (opt)
        case 'b':
            options -> = 1
};


