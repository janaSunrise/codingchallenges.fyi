#include <stdio.h>
#include <ctype.h>

int get_char_count(FILE *fp) {
  int count = 0;
  while (fgetc(fp) != EOF) count++;
  return count;
}

int get_line_count(FILE *fp) {
  int c;
  int count = 0;

  while ((c = fgetc(fp)) != EOF)
    if (c == '\n')
      count++;

  return count;
}

int get_word_count(FILE *fp) {
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

int main(int argc, char* argv[]) {
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
      printf("%d %s\n", get_char_count(file), filepath);
      break;

    case 'l':
      printf("%d %s\n", get_line_count(file), filepath);
      break;

    case 'w':
      printf("%d %s\n", get_word_count(file), filepath);
      break;

    default:
      printf("error: invalid mode flag\n");
  }

  // safely close file when done
  fclose(file);

  return 0;
}

