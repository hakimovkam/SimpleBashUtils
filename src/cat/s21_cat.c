#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define shortFlags "+benstvTE"
//MARK: - struct flags

typedef struct options {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} opt;
static struct option longOption[] = {
    {"number-nonblank", 0, NULL, 'b'},
    {"number", 0, NULL, 'n'},
    {"squeeze-blank", 0, NULL, 's'},
    { NULL, 0, NULL, 0}
};

//MARK: - functions

void reader(FILE *inputFile, FILE *outputFile);
void formater(opt *exemplarOpt, FILE *inputFile, FILE *outputFile); 
int parser(int argc, char *argv[], opt *exemplarOpt);

//MARK: - main

int main(int argc, char *argv[]) {
    
    opt options = {0};
    FILE *fileName;
    int countOfFlags = parser(argc, argv, &options), currentFilePosition = optind;
    
    if (argc == 1) {
        while(1) {
            reader(stdin, stdout);
        }
    } else if (countOfFlags == 0) {
        while (currentFilePosition != argc) {
            if ((fileName = fopen(argv[currentFilePosition], "r")) != NULL) {
                reader(fileName, stdout);
                fclose(fileName);
                currentFilePosition += 1;
            } else {
                printf("cat: %s: No such file or directory\n", argv[currentFilePosition]);
                currentFilePosition += 1;
            }
        }
    }
    return 0;
}

//MARK: - parser

int parser(int argc, char *argv[], opt *exemplarOpt) {
    
    int longIndex = 0, countOfFlags = 0;
    char opt;
    
    while ((opt = getopt_long(argc, argv, shortFlags, longOption, &longIndex)) != -1) {
        switch(opt) {
            case 'b':
                exemplarOpt -> b = 1;
                countOfFlags +=1;
                break;
            case 'e':
                exemplarOpt -> e = 1;
                exemplarOpt -> v = 1;
                countOfFlags +=1;
                break;
            case 'n':
                exemplarOpt -> n = 1;
                countOfFlags +=1;
                break;
            case 's':
                exemplarOpt -> s = 1;
                countOfFlags +=1;
                break;
            case 't':
                exemplarOpt -> t = 1;
                exemplarOpt -> v = 1;
                countOfFlags +=1;
                break;
            default:
                fprintf(stderr, "use correct flags\n");
                exit(1);
        }
    }
    return countOfFlags;
}

//MARK: - reader

void reader(FILE *inputFile, FILE *outputFile) {
    
int ch;
    while ((ch = getc(inputFile)) != EOF)
        putc(ch, outputFile);
}

