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
int parser(int argc, char *argv[], opt *exemplarOpt);
void squeezeBlank(opt exemplarOpt, int *flagForSqueezeBlank, char currentChar, char previousChar, int *enterCounter);
void numberNonblank(opt exemplarOpt, char currentChar, char previousChar, int *lineCounter);
void number(opt exemplarOpt, int *lineCounter, char currentChar, char previousChar);
void flagE(opt exemplarOpt, char currentChar);
//MARK: - main
int main(int argc, char *argv[]) {
    opt options = {0};
    FILE *fileName;
    int countOfFlags = parser(argc, argv, &options), currentFilePosition = optind;
    
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
//MARK: - formater
void formater(opt exemplarOpt, FILE *inputFile) {
    char currentChar, previousChar = '\n';
    int lineStart = 0, lineCounter = 0, enterCounter = 0;
    /* флаг начала строки для b и n, счетчик строк b и n, счетчик пустых строк для s*/
    while((currentChar = fgetc(inputFile)) != EOF) {
        int flagForSqueezeBlank = 0;
        squeezeBlank(exemplarOpt, &flagForSqueezeBlank, currentChar, previousChar, &enterCounter);
        if (flagForSqueezeBlank && enterCounter > 1) continue;
        numberNonblank(exemplarOpt, currentChar, previousChar, &lineCounter);
        number(exemplarOpt, &lineCounter, currentChar, previousChar);
        flagE(exemplarOpt, currentChar);
        
        putc(currentChar, stdout);
        previousChar = currentChar;
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
void flagE(opt exemplarOpt, char currentChar) {
    if (exemplarOpt.e == 1) {
        if (currentChar == '\n') {
            printf("$");
        } else if (currentChar == '\n' && previousChar == '\n') {
            printf("%6d\t", );

        }
    }
}

//void flagV() {}
//
//void flagT() {}



//void formatter(type_flags flags, FILE *f) {
//  int cur_sym, prev_sym = '\n';
//    int line_start = 0; // флаг начала строки для флага b и n
//    int line_counter = 0; // счетчик строк для флага b и n
//    int enter_counter = 0; // счетчик для пустых строк для флага s
//  while ((cur_sym = fgetc(f)) != EOF) {
//    int flag_for_s = 0;
//    flag_s(flags, &flag_for_s, cur_sym, prev_sym, &enter_counter);
//    if (flag_for_s && enter_counter > 1) continue;
//    flag_b(flags, flag_for_s, cur_sym, &line_start, &line_counter);
//    flag_n(flags, prev_sym, &line_counter, &line_start);
//    flag_e(flags, cur_sym);
//    if (flag_t(flags, cur_sym)) continue;
//    if (flag_v(flags, cur_sym)) continue;
//    if (cur_sym == '\n') line_start = 0;
//    putc(cur_sym, stdout);
//    prev_sym = cur_sym;
//  }
//}
