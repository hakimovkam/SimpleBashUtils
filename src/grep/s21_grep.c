#include "s21_grep.h"

int main(int argc, char *argv[]) {
    opt options = {0};
    int fileIndex = 0, templateIndex = 0, optindIndex = 0, flagManyFiles = 0, flagBreakLineInFile = 0;
    char *templates = (char *)calloc(0, sizeof(char));
    parser(argc, argv, &options, &fileIndex, &templateIndex, &optindIndex, &templates, &flagBreakLineInFile);
    FILE *file;
    
    if (options.f == 0 && options.e == 0) {
        writeTemplates('a', argv[templateIndex], &templates, &flagBreakLineInFile);
        flagManyFiles = (argc - optindIndex - 1 == 1) ? 0 : 1;
    } else {
        templates[strlen(templates) - 1] = '\0';
        flagManyFiles = (argc - optindIndex == 1) ? 0 : 1;
    }
        while (fileIndex != argc) {
            if((file = fopen(argv[fileIndex], "r")) != NULL) {
                if (flagBreakLineInFile == 0) searchTemplate(&templates, file, options, argv[fileIndex], flagManyFiles, templateIndex);
                else printFile(file);
                fclose(file);
            }
            else {
                if (options.s == 0) fprintf(stderr, "grep: %s: No such file or directory", argv[fileIndex]);
            }
            fileIndex += 1;
        }
//        int strLenArray =  strlen(templates);
//        for (int i = 0; i < strLenArray; i++) {
//            printf("%c", (templates[i]));
//        }
    return 0;
}

//MARK: - parser
void parser(int argc, char *argv[], opt *exemplarOpt, int *fileIndex, int *templateIndex, int *optindIndex, char **templates,  int *flagBreakLineInFile) {
    int option = 0, longIndex = 0;
    static struct option longOption[] = {{0, 0, 0, 0}};
    while ((option = getopt_long(argc, argv, optionsString, longOption, &longIndex)) != -1) {
        writeOptions(option, exemplarOpt, templates, flagBreakLineInFile);
    }
    *optindIndex = optind;
    if (exemplarOpt->f || exemplarOpt->e) *fileIndex = optind, *templateIndex = 2;
    else *fileIndex = optind + 1, *templateIndex = optind;
}

//MARK: - searchTemplate
void searchTemplate(char **template, FILE *inputFile, opt exemplarOpt, char *fileName, int flagManyFiles, int templateIndex) {
    int t = 0, counterC = 0, lineCounter = 0;
    regex_t re;
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
        
        //флаг f и флаг v не работают корректно вместе
        if (regexec(&re, buffer, 0, NULL, 0) != 0 && (exemplarOpt.v == 1)) {
            if (exemplarOpt.n || exemplarOpt.h || templateIndex) {
                    if (exemplarOpt.c) {
                        counterC += 1;
                        continue;
                    }
                if (exemplarOpt.h == 0 && flagManyFiles && !exemplarOpt.l) printf("%s:", fileName);
                if (exemplarOpt.n) printf("%d:", lineCounter);
                if (exemplarOpt.l != 1) {
                    if (exemplarOpt.o) {  optionsO(buffer, *template, exemplarOpt); } else { printf("%s", buffer); }
                    if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] != '\n' && exemplarOpt.o == 0) { printf("\n"); }
                } else {
                    printf("%s\n", fileName);
                    break;
                }
            }
            } else if (regexec(&re, buffer, 0, NULL, 0) == 0 && (exemplarOpt.v != 1)) {
                if (exemplarOpt.n || exemplarOpt.h || templateIndex) {
                    if (exemplarOpt.c) {
                        if (buffer[0] != '\n') counterC += 1;
                        continue;
                    }
                    if (exemplarOpt.h == 0 && flagManyFiles && exemplarOpt.l == 0) printf("%s:", fileName);
                    if (exemplarOpt.n) printf("%d:", lineCounter);
                    if (exemplarOpt.l != 1) {
                        if (exemplarOpt.o) {  optionsO(buffer, *template, exemplarOpt); } else { printf("%s", buffer); }
                        if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] != '\n' && exemplarOpt.o == 0) { printf("\n"); }
                    } else {
                        printf("%s\n", fileName);
                        break;
                    }
                }
            }
        }
    regfree(&re);
    if (flagManyFiles && exemplarOpt.c && exemplarOpt.h == 0) printf("%s:%d\n", fileName, counterC);
    if ((flagManyFiles == 0 || exemplarOpt.h == 1) && exemplarOpt.c && exemplarOpt.o == 0) printf("%d\n", counterC);
    if (exemplarOpt.l && exemplarOpt.c == 0 && exemplarOpt.l == 0) {
        printf("%s\n", fileName);
    }
}

//MARK: - write options and templates to array
void writeOptions(int option, opt *exemplarOpt, char **templates, int *flagBreakLineInFile) {
    switch(option) {
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
            fprintf(stderr,"use correct flags\n");
            exit(1);
    }
}

void writeTemplates(char option, char *optarg, char **templateName,  int *flagBreakLineInFile) {
    if (option == 'f') {
    FILE *file;
    if ((file = fopen(optarg, "r")) != NULL) {
        char currentChar = '0', previousChar = '\n';
        int strLenArray = strlen(*templateName);
        while ((currentChar = fgetc(file)) != EOF) {
            if (currentChar == '\n' && previousChar != '\n') {
                (*templateName) = (char *)realloc((*templateName), (strLenArray + 1) * sizeof(char));
                (*templateName)[strLenArray] = '|';
                strLenArray += 1;
                previousChar = currentChar;
            } else if (currentChar == '\n' && previousChar == '\n') {
                *flagBreakLineInFile = 1;
            } else {
                (*templateName) = (char *)realloc((*templateName), (strLenArray + 1) * sizeof(char));
                (*templateName)[strLenArray] = currentChar;
                strLenArray += 1;
                previousChar = currentChar;
            }
        }
        (*templateName) =
            (char *)realloc((*templateName), (strLenArray + 1) * sizeof(char));
        (*templateName)[strLenArray] = '|';
        fclose(file);
    } else {
        printf("не удалось открыть файл \n");
    }
    } else if (option == 'e') {
    int strLenTemplate = strlen(optarg), strLenArray = strlen(*templateName),
        index = 0;
    (*templateName) = (char *)realloc((*templateName), (strLenTemplate + strLenArray + 1) * sizeof(char));
    for (int i = strLenArray; i < strLenArray + strLenTemplate + 1; i++) {
        (*templateName)[i] = optarg[index];
        index += 1;
    }
    (*templateName)[strLenArray + strLenTemplate] = '|';
    } else {
    int strLenTemplate = strlen(optarg);
    (*templateName) =
        (char *)realloc((*templateName), strLenTemplate * sizeof(char));
    for (int i = 0; i < strLenTemplate; i++) {
        (*templateName)[i] = optarg[i];
    }
    }
}

//MARK: - -o
void optionsO(char *buffer, char *templates, opt exemplarOpt) {
    regex_t re = {0};
    regmatch_t pmatch[4024];
    int t = 1, ct = 0, i = 0, j = 0;
    char *temp = (char *)calloc(0, sizeof(char));
    char *str = buffer;
    int countTemp = countTemplates(templates);
    
    
    while (ct < countTemp) {
        
        //записали аргументы по очередно в массив
        if (countTemp != 1) {
            while (templates[i] != '|') {
                temp = (char *)realloc(temp, (i+1) * sizeof(char));
                temp[j] = templates[i];

                i += 1;
                j += 1;
            }
        } else {
            int strLenTemp = strlen(templates);
            for (int l = 0; l < strLenTemp; l++) {
                temp = (char *)realloc(temp, (i+1)*sizeof(char));
                temp[l] = templates[i];
                i += 1;
            }
        }

//        regcomp
        if (exemplarOpt.i) {
            if ((t = regcomp(&re, temp, REG_EXTENDED | REG_ICASE)) != 0) {
                printf("failed %d", t);
                regfree(&re);
            }
      } else if (!exemplarOpt.i) {
          if ((t = regcomp(&re, temp, REG_EXTENDED)) != 0) {
              printf("failed %d", t);
              regfree(&re);
          }
    }

        while (!regexec(&re, str, 1, pmatch, 0)) {
          for (int j = pmatch[0].rm_so; j < pmatch[0].rm_eo; ++j)
            printf("%c", str[j]);
          printf("\n");
          str += pmatch[0].rm_eo;
        }
        ct += 1;
        j = 0;
        regfree(&re);
        free(temp);
    }
}

void printFile(FILE *file) {
    char s[512];
    while (fgets(s, 512, file) != NULL) {
        fprintf(stdout, "%s", s);
    }
}

int countTemplates(char *templates) {
    int countTemp = 1, strLenArray =  strlen(templates);
    
    for (int i = 0; i < strLenArray; i++) {
        if (templates[i] == '|') countTemp += 1;
    }
    return countTemp;
}

/*
void flag_o(char **templates, int count_temps, char *str, int cflags) {
  regex_t regex = {0};
  regmatch_t pmatch[1] = {0};
  int i = 0;
  int j = 0;
  char *temp_ = malloc(0);
  while (i < count_temps) {
    while (*templates[j] != '|') {
      temp_ = realloc(temp_, j + 1, sizeof(char));
      temp_[j_temp] = templates[j];
    }
    regcomp(&regex, temp_, cflags);
    while (!regexec(&regex, str, 1, pmatch, 0)) {
      for (int j = pmatch[0].rm_so; j < pmatch[0].rm_eo; ++j)
        printf("%c", str[j]);
      printf("\n");
      str += pmatch[0].rm_eo;
    }
    i++;
    regfree(&regex);
    free(temp_);
    j_temp = 0;
  }
}
 
 
 
 
 void optionsO(char *buffer, char *templates, opt exemplarOpt, int lineCounter) {
   regex_t re;
   regmatch_t pmatch[4024];
   int status = 1;
   char *s = buffer;

   if (exemplarOpt.i) {
     if ((status = regcomp(&re, templates, REG_EXTENDED | REG_ICASE)) != 0) {
       printf("failed %d", status);
       regfree(&re);
     }
   } else if (!exemplarOpt.i) {
     if ((status = regcomp(&re, templates, REG_EXTENDED)) != 0) {
       printf("failed %d", status);
       regfree(&re);
     }
   }

   if (status == 0 && exemplarOpt.v == 0) {
     for (int i = 0; buffer[i] != '\0'; i++) {
       if (regexec(&re, s, 1, pmatch, 0) != exemplarOpt.v) {
         break;
       }
         if (!exemplarOpt.n) {
             printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                                    s + pmatch[0].rm_so);
                             s += pmatch[0].rm_eo;
             
         } else {
             printf("%d:%.*s\n", lineCounter, (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                    s + pmatch[0].rm_so);
             s += pmatch[0].rm_eo;
         }
     }
   }
   regfree(&re);
 }
 
 
 
 
 
 */


