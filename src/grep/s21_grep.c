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
void searchString(char *argv, FILE *inputFile, opt exemplarOpt, char *fileName, int flagManyFiles, int index);

//MARK: -
int main(int argc, char *argv[]) {
    opt options = {0};
    int index = 0;
    parser(argc, argv, &options, &index);
    int flagManyFiles = (argc - 1 - index != 1) ? 1 : 0;
    FILE *file;
    
    if (options.f == 0 && options.e == 0) {
        if((file = fopen(argv[index+1], "r")) != NULL) {
            searchString(argv[index], file, options, argv[index+1], flagManyFiles, index);
        } else {
            if (options.s == 0) fprintf(stderr, "grep: %s: No such file or directory\n", argv[index+1]);
        }
    } else {
        if((file = fopen(argv[index], "r")) != NULL) {
            
        } else {
//            if (exemplarOpt.s != 0) fprintf(stderr, "grep: %s: No such file or directory\n", argv[index]);
        }
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
//    printf("\n%d - %s  argc = %d \n\n\n\n", optind, argv[optind], argc);
}

void searchString(char *argv, FILE *inputFile, opt exemplarOpt, char *fileName, int flagManyFiles, int index) {
    int t = 0, cflags = (exemplarOpt.i) ? REG_ICASE : 0, counterC = 0, lineCounter = 0;
    regex_t re;
    char buffer[bufferSize];
    
    if ((t = regcomp(&re, argv, cflags)) != 0) {
        fprintf( stderr, "grep: %s (%s)\n", buffer, argv );
        return;
    }
    while (fgets(buffer, bufferSize, inputFile) != NULL) {
        lineCounter += 1;
        if (regexec(&re, buffer, 0, NULL, 0) != 0 && exemplarOpt.v == 1) {
                if (exemplarOpt.c || exemplarOpt.n || exemplarOpt.h || index) {
                    counterC += 1;
                    if (exemplarOpt.h == 0 && flagManyFiles) printf("%s:", fileName);
                    if (exemplarOpt.n) printf("%d:", lineCounter);
                    if (exemplarOpt.l != 1) fputs(buffer, stdout);
                }
            } else if (regexec(&re, buffer, 0, NULL, 0) == 0 && exemplarOpt.v == 0) {
                if (exemplarOpt.c || exemplarOpt.n || exemplarOpt.h || index) {
                    counterC += 1;
                    if (exemplarOpt.h == 0 && flagManyFiles) printf("%s:", fileName);
                    if (exemplarOpt.n) printf("%d:", lineCounter);
                    if (exemplarOpt.l != 1) fputs(buffer, stdout);
                }
            }
        }
    regfree(&re);
    if (exemplarOpt.c) printf("%d", counterC);
    if (exemplarOpt.l && exemplarOpt.c == 0) {
        printf("%s", fileName);
    } else if (exemplarOpt.l && exemplarOpt.c) {
        printf("\n%s", fileName);
    }
}


void writeOptions(int option, opt *exemplarOpt) {
    switch(option) {
        case 'e':
            exemplarOpt->e = 1;
            break;
        case 'i':
            exemplarOpt->i = 1;  // +
            break;
        case 'v':
            exemplarOpt->v = 1; // +
            break;
        case 'c':
            exemplarOpt->c = 1; // +
            break;
        case 'l':
            exemplarOpt->l = 1; // +
            break;
        case 'n':
            exemplarOpt->n = 1; // +
            break;
        case 'h': 
            exemplarOpt->h = 1; // +
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

