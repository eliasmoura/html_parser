#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef float real32;
typedef double real64;

#define false 0
#define true 1

#define HTML_TAG_LIST    \
  TAG(HTML_HTML, "html") \
  TAG(HTML_BODY, "body") \
  TAG(HTML_HEAD, "head") \
  TAG(HTML_P, "p")       \
  TAG(HTML_META, "meta")

#define TAG(tag, string) tag,
enum html_tag { HTML_TAG_LIST MAX_HTML_TAGS, HTML_DOC, UNKNOWNTAG, ZEROING };
#undef TAG

#define TAG(tag, string) string,
char *tag_string[] = {HTML_TAG_LIST};
#undef TAG

struct node {
  enum html_tag token;
  bool32 is_closing_token;
  int total_childs;
  struct node *childs;
};
void add_child_node(struct node *child_array, int *size, struct node child) {
  child_array[*size] = child;
  *size += 1;
}
void add_node(struct node *root, int new_node_deph, struct node *new_node){
  if(new_node_deph > 1){
    int index = root->total_childs;
    if(index !=0)
      index -=1;
    add_node(&root->childs[index], --new_node_deph, new_node);
  }else{
  root->childs[root->total_childs] = *new_node;
  root->total_childs +=1;
  }
}
int init_size = 1000;
void alloc_memory(struct node *node) {
  node->childs = calloc(sizeof(struct node), (unsigned int)init_size);
  node->total_childs = 0;
  assert(node->childs != NULL);
}
void print_nodes(struct node *parent, int level){
  char *identing = "  ";
  printf("%s\n", tag_string[parent->token]);
  ++level;
  for(int i = 0; i < parent->total_childs; ++i){
    for (int j = 0; j < level; ++j)
      printf("%s", identing);
    print_nodes(&parent->childs[i], level);
  }
}
int main(int argc, char **argv) {
  char *path;
  if (argc == 2)
    path = argv[1];
  else
    path = "test.html";
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("Couldn't load the file \"%s\"!! Exiting...\n", path);
    return 1;
  }
  printf("Reading file: %s\n", path);
  int element;
  String raw_text;
  string_init(&raw_text);
  while ((element = fgetc(file)) != '\0' && element != EOF) {
    string_append_char(&raw_text, element);
  }

  int *interator = raw_text.data;
  int *start_token = NULL, *end_token = NULL;
  bool32 in_token = false;
  bool32 is_closing_token = false;

  struct node node;
  node.token = HTML_DOC;
  alloc_memory(&node);

  struct node *node_stack;
  node_stack =
      (struct node *)calloc((size_t)init_size, sizeof(struct node));
  assert(node_stack);
  //  alloc_memory(node_stack);
  //  add_child_node(node_stack, node);
  node_stack[0] = node;
  int node_stack_size = 1;
  int deph = 1;
  while (*interator != '\0') {
    if (*interator == '<') {
      if (*(interator + 1) == '/')
        is_closing_token = true;
      else
        is_closing_token = false;
      start_token = interator;
      in_token = true;
      end_token = NULL;
    }

    if (in_token) {
      if (*interator == '>') {
        end_token = interator;
        in_token = false;
      }
    }

    if (end_token != NULL) {
      if (is_closing_token) ++start_token;
      for (int i = 0; i < MAX_HTML_TAGS; i++) {
        if (string_compair_chars(start_token + 1, end_token, tag_string[i]) ==
            0) {
          if (!is_closing_token) {
            struct node new_node;
            //            printf("token: %d", new_node.token);
            alloc_memory(&new_node);
            new_node.token = (enum html_tag)i;
            new_node.is_closing_token = false;
            add_node(&node, deph, &new_node);
            ++deph;
            /* add_child_node(node_stack[node_stack_size - 1].childs, */
            /*                &node_stack[node_stack_size - 1].total_childs, */
            /*                new_node); */
            /* node_stack[node_stack_size] = new_node; */
            node_stack_size += 1;
          } else {
            //            node_stack->total_childs -=1;
            --deph;
            node_stack_size -= 1;
            /* printf("%s\n", tag_string[i]); */
          }
          break;
        }
      }
      end_token = NULL;
    }
    ++interator;
  }
  print_nodes(node.childs, 0);
  /* printf("stack size: %d\n", node_stack[0].childs[0].token); */
  fclose(file);
  free(raw_text.data);
  free(node_stack);
  return 0;
}
