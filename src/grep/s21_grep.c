#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <regex.h>

#define optionsString "e:ivclnhsf:o"
#define bufferSize 512

typedef struct options {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
} opt;

void writeOptions(int option, opt *exemplarOpt);
void parser(int argc, char *argv[], opt *exemplarOpt, int *index);
void searchString(char *argv, FILE *inputFile, opt exemplarOpt, char *fileName);

//MARK: -
int main(int argc, char *argv[]) {
    opt options = {0};
    int index = 0;
    parser(argc, argv, &options, &index);
    
    FILE *file;
    if (options.f == 0 && options.e == 0) {
        if((file = fopen(argv[index+1], "r")) != NULL) {
            searchString(argv[index], file, options, argv[index+1]);
        }
    } else {
        if((file = fopen(argv[index], "r")) != NULL) {}
    }
    return 0;
}

void parser(int argc, char *argv[], opt *exemplarOpt, int *index) {
    int option = 0, longIndex = 0;

    static struct option longOption[] = {{0, 0, 0, 0}};
    
    while ((option = getopt_long(argc, argv, optionsString, longOption, &longIndex)) != -1) {
        writeOptions(option, exemplarOpt);
    }
    
    *index = optind;
//    печать аргументов после getopt_long
//    for (int i = 0; i < argc; i++) {
//        if (i != 0) {
//            printf("%s ", argv[i]);
//        }
//    }
//
//    printf("\n%d - %s\n", optind, argv[optind]);
}


//обрабатывает ситуации без флага, с флагом -i, -v, -c
void searchString(char *argv, FILE *inputFile, opt exemplarOpt, char *fileName) {
    int t = 0, cflags = (exemplarOpt.i) ? REG_ICASE : 0, counterC = 0;
    regex_t re;
    char buffer[bufferSize];
    
    if ((t = regcomp(&re, argv, cflags)) != 0) {
        fprintf( stderr, "grep: %s (%s)\n", buffer, argv );
        return;
    }
        
    while (fgets(buffer, bufferSize, inputFile) != NULL) {
            if (regexec(&re, buffer, 0, NULL, 0) != 0 && exemplarOpt.v == 1) {
                (exemplarOpt.c) ? counterC += 1 : fputs(buffer, stdout);
            } else if (regexec(&re, buffer, 0, NULL, 0) == 0 && exemplarOpt.v == 0) {
                (exemplarOpt.c) ? counterC += 1 : fputs(buffer, stdout);
            }
        }
    regfree(&re);
    if (exemplarOpt.c) printf("%d", counterC);
    if (exemplarOpt.l) printf("\n%s", fileName);
}

void writeOptions(int option, opt *exemplarOpt) {
    switch(option) {
        case 'e':
            exemplarOpt->e = 1;
            break;
        case 'i':
            exemplarOpt->i = 1;
            break;
        case 'v':
            exemplarOpt->v = 1;
            break;
        case 'c':
            exemplarOpt->c = 1;
            break;
        case 'l':
            exemplarOpt->l = 1;
            break;
        case 'n':
            exemplarOpt->n = 1;
            break;
        case 'h':
            exemplarOpt->h = 1;
            break;
        case 's':
            exemplarOpt->s = 1;
            break;
        case 'f':
            exemplarOpt->f = 1;
            break;
        case 'o':
            exemplarOpt->o = 1;
            break;
        default:
            fprintf(stderr,"use correct flags\n");
            exit(1);
    }
}
