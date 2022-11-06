#ifndef SRC_CAT_CATOPTIONS_H
#define SRC_CAT_CATOPTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define shortFlags "+benstvTE"

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

void reader(FILE *inputFile, FILE *outputFile);
void formater(opt exemplarOpt, FILE *inputFile);
void parser(int argc, char *argv[], opt *exemplarOpt);
void squeezeBlank(opt exemplarOpt, int *flagForSqueezeBlank, char currentChar, char previousChar, int *enterCounter);
void numberNonblank(opt exemplarOpt, char currentChar, char previousChar, int *lineCounter);
void number(opt exemplarOpt, int *lineCounter, char previousChar);
void flagE(opt exemplarOpt, char currentChar);
int flagV(opt exemplarOpt,char *currentChar);
int flagT(opt exemplarOpt, char *currentChar);

#endif // SRC_CAT_CATOPTIONS_H
