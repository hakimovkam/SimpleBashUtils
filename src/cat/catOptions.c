#include "catOptions.h"

void reader(FILE *inputFile, FILE *outputFile) {
  int ch;
  while ((ch = getc(inputFile)) != EOF) putc(ch, outputFile);
}

void squeezeBlank(opt exemplarOpt, int *flagForSqueezeBlank, char currentChar,
                  char previousChar, int *enterCounter) {
  if (exemplarOpt.s == 1) {
    if (currentChar == '\n' && previousChar == '\n') {
      *enterCounter += 1;
      *flagForSqueezeBlank = 1;
    } else {
      *enterCounter = 0;
    }
  }
}

void numberNonblank(opt exemplarOpt, char currentChar, char previousChar,
                    int *lineCounter) {
  if (exemplarOpt.b == 1) {
    if (previousChar == '\n' && currentChar != '\n') {
      *lineCounter += 1;
      printf("%6d\t", *lineCounter);
    }
  }
}

void number(opt exemplarOpt, int *lineCounter, char previousChar) {
  if (exemplarOpt.n == 1 && exemplarOpt.b == 0) {
    if (previousChar == '\n') {
      *lineCounter += 1;
      printf("%6d\t", *lineCounter);
    }
  }
}

void flagE(opt exemplarOpt, char currentChar) {
  if (exemplarOpt.e == 1) {
    if (currentChar == '\n') {
      printf("$");
    }
  }
}

int flagV(opt exemplarOpt, char *currentChar) {
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
