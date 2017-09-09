#include "../html_parser.h"
#include <locale.h>
#include <stdio.h>

void print_nodes(struct node *parent, int level) {
  char *identing = "  ";
  printf("%ls", tag_string[parent->token]);
  for (int i = 0; i < parent->attr_size; ++i) {
    printf("\t%ls", parent->attr[i].name.data);
    if (parent->attr[i].value.data)
      printf(" → %ls", parent->attr[i].value.data);
  }
  printf("\n");
  ++level;
  for (int i = 0; i < parent->size; ++i) {
    for (int j = 0; j < level; ++j)
      printf("%s", identing);
    print_nodes(&parent->childs[i], level);
  }
}

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
