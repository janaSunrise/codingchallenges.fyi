#include <stdio.h>

int get_char_count(FILE *fp) {
  int count = 0;
  while (fgetc(fp) != EOF) count++;
  return count;
}

int main(int argc, char* argv[]) {
  // printf("%d\n", argc);
  // for (int i = 0; i < argc; i++) {
  //   printf("%s\n", argv[i]);
  // }

  if (argc != 3) {
    printf("error: invalid number of arguments\n");
    printf("usage: ./mywc <mode> <filepath>\n");
    return 1;
  }

  char mode;
  if (sscanf(argv[1], "-%c", &mode) != 1) {
    printf("error: invalid mode flag\n");
  }

  char* filepath = argv[2];

  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    printf("error: couldn't open file\n");
    return 1;
  }

  switch (mode) {
    case 'c':
      printf("%d %s\n", get_char_count(file), filepath);
      break;

    default:
      printf("error: invalid options. supported options: -c\n");
  }

  // safely close file when done
  fclose(file);

  return 0;
}

