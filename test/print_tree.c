#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include "../html_parser.h"
#include <locale.h>
#include <stdio.h>
#include <sys/stat.h>
#include "string.h"

void print_nodes(struct node *node, int level) {
  char *identing = "  ";

  if( node->token != HTML_DOC) {
    for (int j = 0; j < level; ++j)
      printf("%s", identing);

    if(node->token != HTML_TEXT) {
      printf("<%ls", tag_string[node->token]);
      if(node->token != HTML_COMMENT) {
        for (int i = 0; i < node->attr_size; ++i) {
          printf(" %ls", node->attr[i].name.data);
          if (node->attr[i].value.data)
            printf("=\"%ls\"", node->attr[i].value.data);
        }
        printf(">\n");
      } else {
        printf("%ls", node->attr[0].value.data);
        printf("-->\n");
      }
    } else if(node->attr->value.size) {
      wchar_t ch = ' ';
      string_trim(&node->attr[0].value, 2, &ch);
      printf("%ls\n", node->attr[0].value.data);
    }

    for (int i = 0; i < node->size; ++i) {
      int child_level = level;
      if(!node->self_close)
        ++child_level;
      print_nodes(&node->childs[i], child_level);
    }

    if(node->token != HTML_TEXT && !node->self_close){
      for (int j = 0; j < level; ++j)
        printf("%s", identing);
      printf("</%ls>\n", tag_string[node->token]);
    }
  } else {
    for(int i = 0; i < node->size; ++i)
      print_nodes(node->childs+i, level);
  }
}

int main(int argc, char *argv[]) {
  setlocale(LC_CTYPE, "en_US.utf8");
  FILE *file = NULL;
  String raw_text;

  if (argc == 2) {
    char *path = argv[1];
    fprintf(stderr, "Reading file: %s\n", path);
    file = fopen(path, "r");

    if (file == NULL) {
      fprintf(stderr, "Couldn't load the file \"%s\"!! Exiting...\n", path);
      return 1;
    }

    // NOTE(elias): fseek(file, 0, SEEK_SET); fseek(file, 0, SEEK_END);
    struct stat f_stat = {0}; // makes the fgetwc fail
    // So I'm using fstat instead. That might be better anyway.
    fstat(fileno(file), &f_stat);

    string_init_capacity(&raw_text, (int)f_stat.st_size);
    wint_t wc = 0;
    while ((wc = fgetwc(file)) && !feof(file)) {
      string_append_char(&raw_text, (int)wc);
    }
  } else {
    file = stdin;
    fprintf(stderr, "Reading from stdin\n");
    string_init(&raw_text);
  }

  struct node root = {0};
  node_init(&root, HTML_DOC);
  root.self_close = 1;

  parse(&root, raw_text.data);

  print_nodes(&root, 0);
  fclose(file);
  free(raw_text.data);
  node_free(&root);
  return 0;
}
