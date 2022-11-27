#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc >= 3) {
    opt options = {0};
    int fileIndex = 0, templateIndex = 0, optindIndex = 0, flagManyFiles = 0,
        flagBreakLineInFile = 0;
    char *templates = (char *)calloc(1, sizeof(char));
    parser(argc, argv, &options, &fileIndex, &templateIndex, &optindIndex,
           &templates, &flagBreakLineInFile);
    FILE *file = NULL;

    if (options.f == 0 && options.e == 0) {
      writeTemplates('a', argv[templateIndex], &templates,
                     &flagBreakLineInFile);
      flagManyFiles = (argc - optindIndex - 1 == 1) ? 0 : 1;
    } else {
      templates[strlen(templates) - 1] = '\0';
      flagManyFiles = (argc - optindIndex == 1) ? 0 : 1;
    }
      
    while (fileIndex != argc) {
      if ((file = fopen(argv[fileIndex], "r")) != NULL) {
        if (flagBreakLineInFile == 0) {
          searchTemplate(&templates, file, options, argv[fileIndex],
                         flagManyFiles, templateIndex);
        } else {
          printFile(file);
        }
          fclose(file);
      } else {
        if (options.s == 0)
          fprintf(stderr, "grep: %s: No such file or directory\n",
                  argv[fileIndex]);
      }
      fileIndex += 1;
    }
    free(templates);
    return 0;
  }
}

// MARK: - parser
void parser(int argc, char *argv[], opt *exemplarOpt, int *fileIndex,
            int *templateIndex, int *optindIndex, char **templates,
            int *flagBreakLineInFile) {
  int option = 0, longIndex = 0;
  static struct option longOption[] = {{0, 0, 0, 0}};
  while ((option = getopt_long(argc, argv, optionsString, longOption,
                               &longIndex)) != -1) {
    writeOptions(option, exemplarOpt, templates, flagBreakLineInFile);
  }
  *optindIndex = optind;
  if (exemplarOpt->f || exemplarOpt->e)
    *fileIndex = optind, *templateIndex = 2;
  else
    *fileIndex = optind + 1, *templateIndex = optind;
}

// MARK: - searchTemplate

void searchTemplate(char **template, FILE *inputFile, opt exemplarOpt,
                    char *fileName, int flagManyFiles, int templateIndex) {
  int t = 0, counterC = 0, counterL = 0, lineCounter = 0, lineCounter2 = 0, flagLC = 0;
  regex_t re = {0};
  char buffer[bufferSize] = "0";
  if (exemplarOpt.i || exemplarOpt.f || exemplarOpt.e) {
    if ((t = regcomp(&re, (*template), REG_ICASE | REG_EXTENDED)) != 0) {
      fprintf(stderr, "grep: %s (%s)\n", buffer, (*template));
      return;
    }
  } else if (exemplarOpt.i) {
    if ((t = regcomp(&re, (*template), REG_ICASE)) != 0) {
      fprintf(stderr, "grep: %s (%s)\n", buffer, (*template));
      return;
    }
  } else {
    if ((t = regcomp(&re, (*template), 0)) != 0) {
      fprintf(stderr, "grep: %s (%s)\n", buffer, (*template));
      return;
    }
  }
    
  while (fgets(buffer, bufferSize, inputFile) != NULL) {
    lineCounter += 1;
    if (regexec(&re, buffer, 0, NULL, 0) != 0 && (exemplarOpt.v == 1)) {
        lineCounter2 = lineCounter;
      if (optionsWithV(exemplarOpt, &counterL, &counterC, fileName,
                       &lineCounter, buffer, templateIndex, &flagManyFiles,
                       template))
          
        continue;
    } else if (regexec(&re, buffer, 0, NULL, 0) == 0 && (exemplarOpt.v != 1)) {
        lineCounter2 = lineCounter;
      if (optionsWithOutV(exemplarOpt, &counterL, &counterC, fileName,
                          &lineCounter, buffer, templateIndex, &flagManyFiles,
                          template))
        continue;
    }
  }
  regfree(&re);
    if (lineCounter == lineCounter2) {
//        printf("1123@@\n");
        doBreakLineInLastLine(exemplarOpt, buffer);
    }
  checkSomeOptions(exemplarOpt, &flagManyFiles, &flagLC, &counterC, &counterL,
                   fileName);
    
}

// MARK: - write options to struct and templates to array
void writeOptions(int option, opt *exemplarOpt, char **templates,
                  int *flagBreakLineInFile) {
  switch (option) {
    case 'e':
      exemplarOpt->e = 1;
      writeTemplates('e', optarg, templates, flagBreakLineInFile);
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
      writeTemplates('f', optarg, templates, flagBreakLineInFile);
      break;
    case 'o':
      exemplarOpt->o = 1;
      break;
    default:
      fprintf(stderr, "use correct flags\n");
      exit(1);
  }
}



void writeTemplates(char option, char *optarg, char **templateName,
                    int *flagBreakLineInFile) {
  if (option == 'f') {
    optionF(optarg, templateName, flagBreakLineInFile);
  } else if (option == 'e') {
    optionE(optarg, templateName);
  } else {
    int strLenTemplate = strlen(optarg);
    (*templateName) =
        (char *)realloc((*templateName), (strLenTemplate + 1) * sizeof(char));
    for (int i = 0; i < strLenTemplate; i++) {
      (*templateName)[i] = optarg[i];
    }
    (*templateName)[strLenTemplate] = '\0';
  }
}

// MARK: - regcomp
void getRegcomp(opt exemplarOpt, regex_t *re, char *temp, int *t) {
  if (exemplarOpt.i && (exemplarOpt.f || exemplarOpt.e)) {
    if ((*t = regcomp(re, temp, REG_ICASE | REG_EXTENDED)) != 0) {
      printf("failed %d", *t);
      regfree(re);
    }
  } else if (exemplarOpt.i) {
    if ((*t = regcomp(re, temp, REG_ICASE)) != 0) {
      printf("failed %d", *t);
      regfree(re);
    }
  } else {
    if ((*t = regcomp(re, temp, 0)) != 0) {
      printf("failed %d", *t);
      regfree(re);
    }
  }
}

// MARK: - options

void optionE(char *optarg, char **templateName) {
  int strLenTemplate = strlen(optarg), strLenArray = strlen(*templateName),
      index = 0;
  (*templateName) = (char *)realloc(
      (*templateName), (strLenTemplate + strLenArray + 2) * sizeof(char));
  for (int i = strLenArray; i < strLenArray + strLenTemplate + 1; i++) {
    (*templateName)[i] = optarg[index];
    index += 1;
  }
  (*templateName)[strLenArray + strLenTemplate] = '|';
  (*templateName)[strLenArray + strLenTemplate + 1] = '\0';
}

void optionO(char *buffer, char *templates, opt exemplarOpt) {
  regex_t re = {0};
  regmatch_t pmatch[1] = {0};
  int t = 1, ct = 0, i = 0, j = 0, countTemp = 0;
  char *str = buffer;
  countTemp = getCountTemplates(templates);
    
  while (ct < countTemp) {
      char *temp = (char *)calloc(1, sizeof(char));
      while (templates[i] != '|' && templates[i] != '\0') {
        temp = (char *)realloc(temp, (j + 1) * sizeof(char));
        temp[j] = templates[i];
        i += 1;
        j += 1;
      }
        temp =  (char *)realloc(temp, (j + 1) * sizeof(char));
        temp[j] = '\0';
    getRegcomp(exemplarOpt, &re, temp, &t);
    while (!regexec(&re, str, 1, pmatch, 0)) {
      for (int k = pmatch[0].rm_so; k < pmatch[0].rm_eo; ++k)
        printf("%c", str[k]);
      printf("\n");
      str += pmatch[0].rm_eo;
    }
    ct += 1;
    j = 0;
    regfree(&re);
    i += 1;
    free(temp);
  }
}

void optionF(char *optarg, char **templateName, int *flagBreakLineInFile) {
  FILE *file = NULL;
  if ((file = fopen(optarg, "r")) != NULL) {
    char currentChar = '0', previousChar = '\n';
    int strLenArray = strlen(*templateName);
    while ((currentChar = fgetc(file)) != EOF) {
      if (currentChar == '\n' && previousChar != '\n') {
        (*templateName) =
            (char *)realloc((*templateName), (strLenArray + 1) * sizeof(char));
        (*templateName)[strLenArray] = '|';
        strLenArray += 1;
        previousChar = currentChar;
      } else if (currentChar == '\n' && previousChar == '\n') {
        *flagBreakLineInFile = 1;
      } else {
        (*templateName) =
            (char *)realloc((*templateName), (strLenArray + 1) * sizeof(char));
        (*templateName)[strLenArray] = currentChar;
        strLenArray += 1;
        previousChar = currentChar;
      }
    }
    (*templateName) =
        (char *)realloc((*templateName), (strLenArray + 2) * sizeof(char));
    (*templateName)[strLenArray] = '|';
    (*templateName)[strLenArray + 1] = '\0';
    fclose(file);
  } else {
    printf("не удалось открыть файл \n");
  }
}

int optionsWithV(opt exemplarOpt, int *counterL, int *counterC, char *fileName,
                 int *lineCounter, char *buffer, int templateIndex,
                 int *flagManyFiles, char **template) {
  int flagEnd = 0;
  *counterL += 1;
  if (exemplarOpt.n || exemplarOpt.h || templateIndex) {
    if (exemplarOpt.c) {
      *counterC += 1;
      flagEnd = 1;
    }
    if (!flagEnd) {
      if (exemplarOpt.h == 0 && *flagManyFiles && !exemplarOpt.l)
        printf("%s:", fileName);
      if (exemplarOpt.n && !exemplarOpt.l) printf("%d:", *lineCounter);
      if (exemplarOpt.l != 1) {
        if (exemplarOpt.o && exemplarOpt.v != 1) {
          optionO(buffer, *template, exemplarOpt);
        } else {
          printf("%s", buffer);
        }
        if (buffer[strlen(buffer)] == '\0' &&
            buffer[strlen(buffer) - 1] != '\n' && exemplarOpt.o == 0) {
          printf("\n");
        }
      }
    }
  }
  return flagEnd;
}

int optionsWithOutV(opt exemplarOpt, int *counterL, int *counterC,
                    char *fileName, int *lineCounter, char *buffer,
                    int templateIndex, int *flagManyFiles, char **template) {
  int flagEnd = 0;
  *counterL += 1;
  if (exemplarOpt.n || exemplarOpt.h || templateIndex) {
    if (exemplarOpt.c) {
      if (buffer[0] != '\n') {
        *counterC += 1;
      }
      flagEnd = 1;
    }
    if (!flagEnd) {
      if (exemplarOpt.h == 0 && *flagManyFiles && exemplarOpt.l == 0)
        printf("%s:", fileName);
      if (exemplarOpt.n && !exemplarOpt.l) printf("%d:", *lineCounter);
      if (exemplarOpt.l != 1) {
        if (exemplarOpt.o) {
          optionO(buffer, *template, exemplarOpt);
        } else {
          printf("%s", buffer);
        }
        if (buffer[strlen(buffer)] == '\0' &&
            buffer[strlen(buffer) - 1] != '\n' && exemplarOpt.o == 0) {
          printf("\n");
        }
      }
    }
  }
  return flagEnd;
}

void checkSomeOptions(opt exemplarOpt, int *flagManyFiles, int *flagLC,
                      int *counterC, int *counterL, char *fileName) {
  if (exemplarOpt.c == 1 && exemplarOpt.l == 1 && *counterC != 0) {
    if (*flagManyFiles && exemplarOpt.h == 0) {
      printf("%s:", fileName);
    }
    printf("%d\n", 1);
    *flagLC = 1;
  } else {
    if (*flagManyFiles && exemplarOpt.c && exemplarOpt.h == 0 && *flagLC == 0)
      printf("%s:%d\n", fileName, *counterC);
    if ((*flagManyFiles == 0 || exemplarOpt.h == 1) && exemplarOpt.c &&
        *flagLC == 0)
      printf("%d\n", *counterC);
  }
  if (exemplarOpt.l && *counterL > 0) {
    printf("%s\n", fileName);
  }
}

// MARK: - some functions

void printFile(FILE *file) {
  char s[512] = "0";
  while (fgets(s, 512, file) != NULL) {
    fprintf(stdout, "%s", s);
  }
}

int getCountTemplates(char *templates) {
  int countTemp = 1, strLenArray = strlen(templates);

  for (int i = 0; i < strLenArray; i++) {
    if (templates[i] == '|') countTemp += 1;
  }
  return countTemp;
}

void doBreakLineInLastLine(opt exemplarOpt, char *buffer) {
    int i = 0;
    
    if (exemplarOpt.v) {
//        printf("do.v\n");
        if (!exemplarOpt.c && ! exemplarOpt.l) {
//            printf("do.v.proverka\n");
            while (buffer[i] != '\0') {
                if (buffer[i + 1] == '\0' && buffer[0] != '\n') {
                    if (buffer[i] != '\n') {
                    printf("\n");
                    }
                }
                i += 1;
            }
        }
    } else {
        if(!exemplarOpt.c && !exemplarOpt.l && !exemplarOpt.v && !exemplarOpt.o) {
      
            while(buffer[i] != '\0') {
                if (buffer[i + 1] == '\0' && buffer [0] != '\n') {
                    if (buffer[i] != '\n') {
                    printf("\n");
                    }
                }
                i += 1;
            }
        }
    }
}

//void nFinder(grepStruct* newGrep, char* buffer) {
//  if (newGrep->c == 0 && newGrep->l == 0 && newGrep->v == 0 &&
//      newGrep->o == 0) {
//    int z = 0;
//    while (buffer[z] != '\0') {
//      if (buffer[z + 1] == '\0' && buffer[0] != '\n') {
//        if (buffer[z] != '\n') {
//          fprintf(stdout, "\n");
//        }
//      }
//      z++;
//    }
//  }
//}

// v
// !v { o }

//если есть флаг в  с и л равно нуль (во всех случаях) если нет то во всех кроме о      с в л о равно нуль
//void nFinderBezV(grepStruct* newGrep, char* buffer) {
//  if (newGrep->c == 0 && newGrep->l == 0) {
//    int z = 0;
//    while (buffer[z] != '\0') {
//      if (buffer[z + 1] == '\0' && buffer[0] != '\n') {
//        if (buffer[z] != '\n') {
//          fprintf(stdout, "\n");
//        }
//      }
//      z++;
//    }
//  }
//}
