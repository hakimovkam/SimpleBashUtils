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
void formater(opt exemplarOpt, FILE *inputFile);
void parser(int argc, char *argv[], opt *exemplarOpt);
void squeezeBlank(opt exemplarOpt, int *flagForSqueezeBlank, char currentChar, char previousChar, int *enterCounter);
void numberNonblank(opt exemplarOpt, char currentChar, char previousChar, int *lineCounter);
void number(opt exemplarOpt, int *lineCounter, char currentChar, char previousChar);
void flagE(opt exemplarOpt, char currentChar,char previousChar);
int flagV(opt exemplarOpt,char *currentChar);
int flagT(opt exemplarOpt, char *currentChar);


//MARK: - main
int main(int argc, char *argv[]) {
    opt options = {0};
    FILE *fileName;
    parser(argc, argv, &options);
    int currentFilePosition = optind;
    
    if (argc == 1) {
        while(1) {
            reader(stdin, stdout);
        }
    } else {
        while (currentFilePosition != argc) {
            if ((fileName = fopen(argv[currentFilePosition], "r")) != NULL) {
                formater(options, fileName);
                fclose(fileName);
                currentFilePosition += 1;
            } else {
                fprintf(stderr, "cat: %s: No such file or directory\n", argv[currentFilePosition]);
                currentFilePosition += 1;
            }
        }
    }
    return 0;
}


//MARK: - parser
void parser(int argc, char *argv[], opt *exemplarOpt) {
    
    int longIndex = 0;
    char opt;
    
    while ((opt = getopt_long(argc, argv, shortFlags, longOption, &longIndex)) != -1) {
        switch(opt) {
            case 'b':
                exemplarOpt -> b = 1;
                break;
            case 'e':
                exemplarOpt -> e = 1;
                exemplarOpt -> v = 1;
                break;
            case 'n':
                exemplarOpt -> n = 1;
                break;
            case 's':
                exemplarOpt -> s = 1;
                break;
            case 't':
                exemplarOpt -> t = 1;
                exemplarOpt -> v = 1;
                break;
            case 'v':
                exemplarOpt -> v = 1;
                break;
            default:
                fprintf(stderr, "use correct flags\n");
                exit(1);
        }
    }
}


//MARK: - reader
void reader(FILE *inputFile, FILE *outputFile) {
    
int ch;
    while ((ch = getc(inputFile)) != EOF)
        putc(ch, outputFile);
}


//MARK: - formater
void formater(opt exemplarOpt, FILE *inputFile) {
    char currentChar, previousChar = '\n';
    int lineCounter = 0, enterCounter = 0;
    /* флаг начала строки для b и n, счетчик строк b и n, счетчик пустых строк для s*/
    while((currentChar = fgetc(inputFile)) != EOF) {
        int flagForSqueezeBlank = 0;
        squeezeBlank(exemplarOpt, &flagForSqueezeBlank, currentChar, previousChar, &enterCounter);
        if (flagForSqueezeBlank && enterCounter > 1) continue;
        numberNonblank(exemplarOpt, currentChar, previousChar, &lineCounter);
        number(exemplarOpt, &lineCounter, currentChar, previousChar);
        flagE(exemplarOpt, currentChar, previousChar);
        previousChar = currentChar;
        if (flagT(exemplarOpt, &currentChar)) continue;
        if (flagV(exemplarOpt, &currentChar)) continue;
        if (currentChar == '\n') lineStart = 0;
        putc(currentChar, stdout);

    }
}


//MARK: - squeezeBlank & -s
void squeezeBlank(opt exemplarOpt, int *flagForSqueezeBlank, char currentChar, char previousChar, int *enterCounter) {
    if (exemplarOpt.s == 1) {
        if (currentChar == '\n' && previousChar == '\n') {
            *enterCounter += 1;
            *flagForSqueezeBlank = 1;
        } else {
            *enterCounter = 0;
        }
    }
}


//MARK: - numberNonblank & -b
void numberNonblank(opt exemplarOpt, char currentChar, char previousChar, int *lineCounter) {
    if (exemplarOpt.b == 1) {
        if (previousChar == '\n' && currentChar != '\n') {
            *lineCounter += 1;
            printf("%6d\t", *lineCounter);
        }
    }
}


//MARK: - number & -n
void number(opt exemplarOpt, int *lineCounter, char currentChar, char previousChar) {
    if (exemplarOpt.n == 1 && exemplarOpt.b == 0) {
        if (previousChar == '\n') {
            *lineCounter += 1;
            printf("%6d\t", *lineCounter);
        }
    }
}


//MARK: - -e
void flagE(opt exemplarOpt, char currentChar, char previousChar) {
    if (exemplarOpt.e == 1) {
        if (currentChar == '\n') {
            printf("$");
        }
    }
}


//MARK: - -v
int flagV(opt exemplarOpt,char *currentChar) {
    int flagResult = 0;
    if (exemplarOpt.v == 1) {
        if (*currentChar < 32 && *currentChar != '\n' && *currentChar != '\t') {
            printf("^%c", *currentChar + 64);
            flagResult = 1;
        } else if (*currentChar == 127) {
            printf("^?");
            flagResult = 1;
        }
    }
    return flagResult;
}


//MARK: - -t
int flagT(opt exemplarOpt, char *currentChar) {
    int flagResult = 0;
    if (exemplarOpt.t == 1) {
        if (*currentChar == '\t') {
            printf("^I");
            flagResult = 1;
        }
    }
    return flagResult;
}
