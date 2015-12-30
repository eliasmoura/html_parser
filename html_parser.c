#include <stdio.h>
#include <string.h>

#include <assert.h>
#include "html_parser.h"

#define HTML_TAG_LIST\
  TAG(HTML, "html")   \
  TAG(BODY, "body")   \
  TAG(HEAD, "head")   \
  TAG(P   , "p"   )   \

#define TAG(tag, tag_name) tag,
enum HTML_TAG {
  HTML_TAG_LIST html_list_size
};
#undef TAG
#define TAG(tag, tag_name) tag_name,
char *tag_name[] = {
  HTML_TAG_LIST
};
#undef TAG

int main(int argc, char** args){
  char* filename;
  if(argc > 1)
    filename = args[1];
  else
    filename = "test.html";
  FILE* file = fopen(filename, "r");
  int element;
  NodeTree node;

  if(file != NULL)
  while((element = fgetc(file )) != EOF){
    if(element == '<'){
      int index = 0;
      char token[10];
      while((element = fgetc(file)) != EOF && element != ' ' && element != '>'){
        token[index] = element;
        index +=1;
      }
      for(int i = 0; i < html_list_size; i++)
        if(strcmp(token, tag_name[i]) ==0 ){
          node[node_index] = i;
          node_index +=1;
      }
      for(int i = 0; i <10; i++)
        token[i] = 0;
  }
  }
  else
    printf("Coudn't find the file!\n");
  printf("html nodes: %d\n", node_index);
  for(int i = 0; i < node_index; i++)
    printf("%s\n", tag_name[node[i]]);
  fclose(file);
  return 0;
}
