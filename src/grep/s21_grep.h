#ifndef SRC_S21_GREP_H
#define SRC_S21_GREP_H

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

void writeOptions(int option, opt *exemplarOpt, char **templates, int *flagBreakLineInFile);
void parser(int argc, char *argv[], opt *exemplarOpt, int *fileIndex, int *templateIndex, int *optindIndex, char **templates, int *flagBreakLineInFile);
void searchTemplate(char **template, FILE *inputFile, opt exemplarOpt, char *fileName, int flagManyFiles, int templateIndex);
void writeTemplates(char option, char *optarg, char **templateName, int *flagBreakLineInFile);
void optionsO(char *buffer, char *templates, opt exemplarOpt);
void printFile(FILE *file);
int countTemplates(char *templates);


#endif // SRC_S21_GREP_H
