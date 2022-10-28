#include <stdio.h>
#include <string.h>

//MARK: - struct flags
    /// это нужно будет закинуть в s21_cat.h

typedef struct {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} flag;


//MARK: - main

int main(int argc, char *argv[]) {
    FILE *f;
    void filecopy(FILE *, FILE *);
    if (argc == 1) {
        filecopy(stdin, stdout)
    } else {
        flag config = {0};
        
        while (--argc > 0) {
            if ((f = fopen(*++argv, "r")) == NULL) {
                printf("cat can not open the file %s\n", *argv);
                return 1;
            } else {
                filecopy(f, stdout);
                fclose(f);
            }
        }
    }
    return 0;
}

void filecopy(FILE *ifp, FILE *ofp) {
int c;
    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);
}



