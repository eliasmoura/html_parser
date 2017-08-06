#include <locale.h>
#include <stdio.h>
#include "../html_parser.h"

int main(int argc, char *argv[]) {
  setlocale(LC_CTYPE, "en_US.utf8");
  FILE *file = NULL;
  if (argc == 2) {
    char *path = argv[1];
    printf("Reading file: %s\n", path);
    file = fopen(path, "r");
    if (file == NULL) {
      printf("Couldn't load the file \"%s\"!! Exiting...\n", path);
      return 1;
    }
  } else {
    file = stdin;
    fprintf(stderr, "Reading from stdin\n");
  }

  String raw_text;
  string_init(&raw_text);
  wint_t c = 0;
  while ((c = fgetwc(file)) && c != WEOF) {
    string_append_char(&raw_text, (int)c);
  }

  struct node root = {0};
  node_init(&root, HTML_DOC);
  root.self_close = 1;

  parse(&root, raw_text.data);

  print_nodes(&root, 0);
  fclose(file);
  free(raw_text.data);
  free_nodes(&root);
  return 0;
}
