#include "catOptions.h"

int main(int argc, char *argv[]) {
  opt options = {0};
  FILE *fileName;
  parser(argc, argv, &options);
  int currentFilePosition = optind;

  if (argc == 1) {
    while (1) {
      reader(stdin, stdout);
    }
  } else {
    while (currentFilePosition != argc) {
      if ((fileName = fopen(argv[currentFilePosition], "r")) != NULL) {
        formater(options, fileName);
        fclose(fileName);
        currentFilePosition += 1;
      } else {
        fprintf(stderr, "cat: %s: No such file or directory\n",
                argv[currentFilePosition]);
        currentFilePosition += 1;
      }
    }
  }
  return 0;
}

void parser(int argc, char *argv[], opt *exemplarOpt) {
  int longIndex = 0;
  char opt;

  while ((opt = getopt_long(argc, argv, shortFlags, longOption, &longIndex)) !=
         -1) {
    switch (opt) {
      case 'b':
        exemplarOpt->b = 1;
        break;
      case 'e':
        exemplarOpt->e = 1;
        exemplarOpt->v = 1;
        break;
      case 'n':
        exemplarOpt->n = 1;
        break;
      case 's':
        exemplarOpt->s = 1;
        break;
      case 't':
        exemplarOpt->t = 1;
        exemplarOpt->v = 1;
        break;
      case 'v':
        exemplarOpt->v = 1;
        break;
      default:
        fprintf(stderr, "use correct flags\n");
        exit(1);
    }
  }
}

void formater(opt exemplarOpt, FILE *inputFile) {
  char currentChar, previousChar = '\n';
  int lineCounter = 0, enterCounter = 0;
  /* флаг начала строки для b и n, счетчик строк b и n, счетчик пустых строк для
   * s*/
  while ((currentChar = fgetc(inputFile)) != EOF) {
    int flagForSqueezeBlank = 0;
    squeezeBlank(exemplarOpt, &flagForSqueezeBlank, currentChar, previousChar,
                 &enterCounter);
    if (flagForSqueezeBlank && enterCounter > 1) continue;
    numberNonblank(exemplarOpt, currentChar, previousChar, &lineCounter);
    number(exemplarOpt, &lineCounter, previousChar);
    flagE(exemplarOpt, currentChar);
    previousChar = currentChar;
    if (flagT(exemplarOpt, &currentChar)) continue;
    if (flagV(exemplarOpt, &currentChar)) continue;
    putc(currentChar, stdout);
  }
}
