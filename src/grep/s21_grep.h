#ifndef SRC_S21_GREP_H
#define SRC_S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void writeOptions(int option, opt *exemplarOpt, char **templates,
                  int *flagBreakLineInFile);
void parser(int argc, char *argv[], opt *exemplarOpt, int *fileIndex,
            int *templateIndex, int *optindIndex, char **templates,
            int *flagBreakLineInFile);
void searchTemplate(char **template, FILE *inputFile, opt exemplarOpt,
                    char *fileName, int flagManyFiles, int templateIndex);
void writeTemplates(char option, char *optarg, char **templateName,
                    int *flagBreakLineInFile);
void optionO(char *buffer, char *templates, opt exemplarOpt);
void printFile(FILE *file);
int getCountTemplates(char *templates);
int optionsWithV(opt exemplarOpt, int *counterL, int *counterC, char *fileName,
                 int *lineCounter, char *buffer, int templateIndex,
                 int *flagManyFiles, char **template,
                 int *doBreakLineInLastLineFlag);
int optionsWithOutV(opt exemplarOpt, int *counterL, int *counterC,
                    char *fileName, int *lineCounter, char *buffer,
                    int templateIndex, int *flagManyFiles, char **template,
                    int *doBreakLineInLastLineFlag);
void checkSomeOptions(opt exemplarOpt, int *flagManyFiles, int *flagLC,
                      int *counterC, int *counterL, char *fileName);
void getRegcomp(opt exemplarOpt, regex_t *re, char *temp, int *t);
void optionF(char *optarg, char **templateName, int *flagBreakLineInFile);
void optionE(char *optarg, char **templateName);
void doBreakLineInLastLine(opt exemplarOpt, char *buffer,
                           int *doBreakLineInLastLineFlag);

#endif  // SRC_S21_GREP_H
