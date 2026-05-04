#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h> // wide character handling. `fgetwc`, similar to `fgetc`

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

// single-pass counter so the default `wc <file>` doesn't need to rewind
void count_all(FILE *fp, int *lines, int *words, int *chars) {
  int c;
  int in_word = 0;
  *lines = *words = *chars = 0;

  while ((c = fgetc(fp)) != EOF) {
    (*chars)++;
    if (c == '\n') (*lines)++;
    if (isspace(c)) {
      in_word = 0;
    } else if (!in_word) {
      in_word = 1;
      (*words)++;
    }
  }
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");

  /* argument formats:
   *   wc                -> default counts on stdin
   *   wc <file>         -> default counts on file
   *   wc -<m>           -> mode counts on stdin (e.g. `cmd | wc -c`)
   *   wc -<m> <file>    -> mode counts on file */

  char mode = 0;
  const char *filepath = "";
  FILE *file = NULL;

  int argi = 1;
  if (argi < argc && argv[argi][0] == '-') {
    if (sscanf(argv[argi], "-%c", &mode) != 1) {
      fprintf(stderr, "error: invalid mode flag. supported: -c, -l, -w, -m\n");
      return 1;
    }
    argi++;
  }

  if (argi < argc) {
    filepath = argv[argi];
    file = fopen(filepath, "r");
    if (file == NULL) {
      fprintf(stderr, "error: couldn't open file '%s'\n", filepath);
      return 1;
    }
  } else {
    file = stdin;
  }

  int rc = 0;
  if (mode == 0) {
    int lines, words, chars;
    count_all(file, &lines, &words, &chars);
    printf("%d %d %d %s\n", lines, words, chars, filepath);
  } else {
    int n = 0;
    int ok = 1;
    switch (mode) {
      case 'c': n = count_chars(file); break;
      case 'l': n = count_lines(file); break;
      case 'w': n = count_words(file); break;
      case 'm': n = count_multibyte_chars(file); break;
      default:
        fprintf(stderr, "error: invalid mode flag '-%c'\n", mode);
        ok = 0;
        rc = 1;
    }
    if (ok) printf("%d %s\n", n, filepath);
  }

  if (file != stdin) fclose(file);
  return rc;
}
