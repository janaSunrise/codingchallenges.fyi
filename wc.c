#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>

int count_chars(FILE *fp) {
  int count = 0;
  while (fgetc(fp) != EOF) count++;
  return count;
}

int count_lines(FILE *fp) {
  int c;
  int count = 0;

  while ((c = fgetc(fp)) != EOF)
    if (c == '\n')
      count++;

  return count;
}

int count_words(FILE *fp) {
  int c;
  int count = 0;
  int in_word = 0;

  while ((c = fgetc(fp)) != EOF) {
    if (isspace(c)) {
      in_word = 0;
    } else if (!in_word) {
      in_word = 1;
      count++;
    }
  }

  return count;
}

int count_multibyte_chars(FILE *fp) {
  int count = 0;
  while (fgetwc(fp) != WEOF) count++;
  return count;
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");

  if (argc != 3) {
    printf("error: invalid number of arguments\n");
    printf("usage: ./mywc <mode> <filepath>\n");
    return 1;
  }

  char mode;
  if (sscanf(argv[1], "-%c", &mode) != 1) {
    printf("error: invalid mode flag. supported flags: -c, -l, -w\n");
  }

  char* filepath = argv[2];

  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    printf("error: couldn't open file\n");
    return 1;
  }

  switch (mode) {
    case 'c':
      printf("%d %s\n", count_chars(file), filepath);
      break;

    case 'l':
      printf("%d %s\n", count_lines(file), filepath);
      break;

    case 'w':
      printf("%d %s\n", count_words(file), filepath);
      break;

    case 'm':
      printf("%d %s\n", count_multibyte_chars(file), filepath);
      break;

    default:
      printf("error: invalid mode flag\n");
  }

  // safely close file when done
  fclose(file);

  return 0;
}

