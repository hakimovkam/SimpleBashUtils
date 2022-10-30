#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define shortFlags "+benstvTE"
//MARK: - struct flags
    /// это нужно будет закинуть в хэдер

typedef struct options {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} opt;

//MARK: - functions

void fileCopy(FILE *, FILE *);
int parser(int argc, char *argv[], opt *exemplarOpt);
void numberNonblank();

//MARK: - main

int main(int argc, char *argv[]) {
    FILE *f;
    if (argc == 1) {
        fileCopy(stdin, stdout);
    } else if (argc == 2) {
        while (--argc > 0) {
            if ((f = fopen(*++argv, "r")) == NULL) {
                printf("cat can not open the file %s\n", *argv);
                return 1;
            } else {
                fileCopy(f, stdout);
                fclose(f);
            }
        }
    } else {
        opt exemplarOpt = {0}; // сразу зануляем все значения в структуре exemplarOpt
        int countOfFlags = parser(argc, argv, &exemplarOpt);
        printf("%d", countOfFlags);
    }
    return 0;
}

//MARK: - fileCopy
///     просто копирование текста из файла в окно терминала
void fileCopy(FILE *ifp, FILE *ofp) {
int c;
    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);
}

//MARK: - parser
///         тут происходит считывание флагов и заполнение экземпляра структуры opt
int parser(int argc, char *argv[], opt *exemplarOpt) {
    
    int longIndex = 0, countOfFlags = 0; //указатель на переменную, в которую будет помещен индекс текущего параметра из массива longOption.
    char opt;
    
    static struct option longOption[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        { NULL, 0, NULL, 0}
    };
    
    /* выше создан массив экземпляров структур, которые описаны в библиотеке getopt.h
         struct option {
         const char *name;
         int has_arg;
         int *flag;
         int val;
     }; */
    
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

//MARK: - realization of flags
///     дальше описываются функции флагов, их лучше вынести в отдельный хедер файл

void numberNonblank() {
    
}


