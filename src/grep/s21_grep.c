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

void writeOptions(int option, opt *exemplarOpt, char **templates);
void parser(int argc, char *argv[], opt *exemplarOpt, int *fileIndex, int *templateIndex, int *optindIndex, char **templates);
void searchTemplate(char **template, FILE *inputFile, opt exemplarOpt, char *fileName, int flagManyFiles, int templateIndex);
void writeTemplatesToArray(char option, char *optarg, char **templateName);
void checkOptions(opt exemplarOpt, int *counterC, int lineCounter, int flagManyFiles, char *fileName, char *buffer);

//MARK: - main
int main(int argc, char *argv[]) {
    opt options = {0};
    int fileIndex = 0, templateIndex = 0, optindIndex = 0;
    char *templates = (char *)calloc(0, sizeof(char));
    parser(argc, argv, &options, &fileIndex, &templateIndex, &optindIndex, &templates);
    int flagManyFiles = (argc - 1 - templateIndex != 1) ? 1 : 0;
    FILE *file;
  
    if (options.f == 0 && options.e == 0) {
        writeTemplatesToArray('a', argv[templateIndex], &templates);
    } else {
        templates[strlen(templates)-1] = '\0';
    }
    while (fileIndex != argc) {
        if((file = fopen(argv[fileIndex], "r")) != NULL) {
            searchTemplate(&templates, file, options, argv[fileIndex], flagManyFiles, templateIndex);
            fclose(file);
        } else {
            if (options.s == 0) fprintf(stderr, "grep: %s: No such file or directory", argv[fileIndex]);
        }
        fileIndex += 1;
        printf("\n");
    }
    
//    int strLenArray =  strlen(templates);
//    for (int i = 0; i < strLenArray; i++) {
//        printf("%c", (templates[i]));
//    }
    return 0;
}

//MARK: - parser
void parser(int argc, char *argv[], opt *exemplarOpt, int *fileIndex, int *templateIndex, int *optindIndex, char **templates) {
    int option = 0, longIndex = 0;
    static struct option longOption[] = {{0, 0, 0, 0}};
    while ((option = getopt_long(argc, argv, optionsString, longOption, &longIndex)) != -1) {
        writeOptions(option, exemplarOpt, templates);
    }
    
    *optindIndex = optind;
    if (exemplarOpt->f || exemplarOpt->e) *fileIndex = optind, *templateIndex = 2;
    else *fileIndex = optind + 1, *templateIndex = optind;
}

//MARK: - searchTemplate
void searchTemplate(char **template, FILE *inputFile, opt exemplarOpt, char *fileName, int flagManyFiles, int templateIndex) {
    int t = 0, counterC = 0, lineCounter = 0, cflags = 0;
    regex_t re;
    char buffer[bufferSize];
    if (exemplarOpt.i || (exemplarOpt.f || exemplarOpt.e)) {
        if ((t = regcomp(&re, (*template), REG_ICASE | REG_EXTENDED)) != 0) {
            fprintf( stderr, "grep: %s (%s)\n", buffer, (*template));
            return;
        }
    } else if (exemplarOpt.i) {
        if ((t = regcomp(&re, (*template), REG_ICASE )) != 0) {
            fprintf( stderr, "grep: %s (%s)\n", buffer, (*template));
            return;
        }
    } else {
        if ((t = regcomp(&re, (*template), 0)) != 0) {
            fprintf( stderr, "grep: %s (%s)\n", buffer, (*template));
            return;
        }
    }
    while (fgets(buffer, bufferSize, inputFile) != NULL) {
        lineCounter += 1;
        if (regexec(&re, buffer, 0, NULL, 0) != 0 && exemplarOpt.v == 1) {
                if (exemplarOpt.c || exemplarOpt.n || exemplarOpt.h || templateIndex) {
                    checkOptions(exemplarOpt, &counterC, lineCounter, flagManyFiles, fileName, buffer);
                }
            } else if (regexec(&re, buffer, 0, NULL, 0) == 0 && exemplarOpt.v == 0) {
                if (exemplarOpt.c || exemplarOpt.n || exemplarOpt.h || templateIndex) {
                    checkOptions(exemplarOpt, &counterC, lineCounter, flagManyFiles, fileName, buffer);
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

void checkOptions(opt exemplarOpt, int *counterC, int lineCounter, int flagManyFiles, char *fileName, char *buffer) {
    *counterC += 1;
    if (exemplarOpt.h == 0 && flagManyFiles) printf("%s:", fileName);
    if (exemplarOpt.n) printf("%d:", lineCounter);
    if (exemplarOpt.l != 1) fputs(buffer, stdout);
    
}
//MARK: - write options and templates to array
void writeOptions(int option, opt *exemplarOpt, char **templates) {
    switch(option) {
        case 'e':
            exemplarOpt->e = 1;
            writeTemplatesToArray('e', optarg, templates);
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
            writeTemplatesToArray('f', optarg, templates);
            break;
        case 'o':
            exemplarOpt->o = 1;
            break;
        default:
            fprintf(stderr,"use correct flags\n");
            exit(1);
    }
}


void writeTemplatesToArray(char option, char *optarg, char **templateName) {
    if (option == 'f') {
        FILE *file;
        if((file = fopen(optarg, "r")) != NULL) {
            char currentChar;
            int strLenArray = strlen(*templateName);
            while ((currentChar = fgetc(file)) != EOF) {
                (*templateName) = (char*)realloc((*templateName),(strLenArray+1)*sizeof(char));
                (*templateName)[strLenArray] = currentChar;
                strLenArray += 1;
            }
            (*templateName) = (char*)realloc((*templateName),(strLenArray+1)*sizeof(char));
            (*templateName)[strLenArray] = '|';
            fclose(file);
        } else {
            printf("не удалось открыть файл \n");
        }
    } else if (option == 'e') {
        int strLenTemplate = strlen(optarg), strLenArray = strlen(*templateName), index = 0;
        (*templateName) = (char*)realloc((*templateName),(strLenTemplate+strLenArray+1)*sizeof(char));
        for (int i = strLenArray; i < strLenArray+strLenTemplate+1; i++) {
            (*templateName)[i] = optarg[index];
            index += 1;
        }
        (*templateName)[strLenArray+strLenTemplate] = '|';
    } else {
        int strLenTemplate = strlen(optarg), index = 0;
        (*templateName) = (char*)realloc((*templateName), strLenTemplate * sizeof(char));
        for (int i = 0; i < strLenTemplate; i++) {
            (*templateName)[i] = optarg[i];
            index += 1;
        }
    }
}
