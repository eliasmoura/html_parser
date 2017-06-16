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
#define INIT_SIZE  1000

//TAG(enum, string, self_close, ver)
//NOTE: ver is for checking if the tag is depracated(0)/HTML5 only(1)/both(2)
//NOTE: self_close: 0 has a </> close tag, 1 does not
#define HTML_TAG_LIST                             \
  TAG(HTML_UNKNOWNTAG,"HTNL_UNKNOWNTAG",1,  0)        \
  TAG(HTML_DOCTYPE,  "!DOCTYPE",    1,  1)        \
  TAG(HTML_COMMENT,   "!--",        1,  2)        \
  TAG(HTML_A,         "a",          0,  2)  \
  TAG(HTML_ABBR,      "abbr",       0,  2)  \
  TAG(HTML_ACRONYM,   "acronym",    0,  0)  \
  TAG(HTML_ADDRESS,   "address",    0,  2)  \
  TAG(HTML_APPLET,    "applet",     0,  0)   \
  TAG(HTML_AREA,      "area",       0,  2)   \
  TAG(HTML_ARTICLE,   "article",    0,  1)   \
  TAG(HTML_ASIDE,     "aside",      0,  1)   \
  TAG(HTML_AUDIO,     "audio",      0,  1)   \
  TAG(HTML_B,         "b",          0,  2)  \
  TAG(HTML_BASE,      "base",       0,  2) \
  TAG(HTML_BASEFONT,  "basefont",   0,  0) \
  TAG(HTML_BDI,       "bdi",        0,  1) \
  TAG(HTML_BDO,       "bdo",        0,  0) \
  TAG(HTML_BIG,       "big",        0,  0) \
  TAG(HTML_BLOCKQUOTE,"blockquote", 0,  0) \
  TAG(HTML_BODY,      "body",       0,  0) \
  TAG(HTML_BR,        "br",         1,  0) \
  TAG(HTML_BUTTON,    "button",     0,  0) \
  TAG(HTML_CANVAS,    "canvas",     0,  0) \
  TAG(HTML_CAPTION,   "caption",    0,  0) \
  TAG(HTML_CENTER,    "center",     0,  0) \
  TAG(HTML_CITE,      "cite",       0,  0) \
  TAG(HTML_CODE,      "code",       0,  0) \
  TAG(HTML_COL,       "col",        0,  0) \
  TAG(HTML_COLGROUP,  "colgroup",   0,  0) \
  TAG(HTML_DATALIST,  "datalist",   0,  0) \
  TAG(HTML_DD,        "dd",         0,  0) \
  TAG(HTML_DEL,       "del",        0,  0) \
  TAG(HTML_DETAILS,   "details",    0,  0) \
  TAG(HTML_DFN,       "dfn",        0,  0) \
  TAG(HTML_DIALOG,    "dialog",     0,  0) \
  TAG(HTML_DIR,       "dir",        0,  0) \
  TAG(HTML_DIV,       "div",        0,  0) \
  TAG(HTML_DL,        "dl",         0,  0) \
  TAG(HTML_DT,        "dt",         0,  0) \
  TAG(HTML_EM,        "em",         0,  0) \
  TAG(HTML_EMBED,     "embed", 0, 0) \
  TAG(HTML_FIELDSET,  "fieldset", 0, 0) \
  TAG(HTML_FIGCAPTION,"figcaption", 0, 0) \
  TAG(HTML_FIGURE,    "figure", 0, 0) \
  TAG(HTML_FONT,      "font", 0, 0) \
  TAG(HTML_FOOTER,    "footer", 0, 0) \
  TAG(HTML_FORM,      "form", 0, 0) \
  TAG(HTML_FRAME,     "frame", 0, 0) \
  TAG(HTML_FRAMESET,  "frameset", 0, 0) \
  TAG(HTML_H1,        "h1", 0, 0) \
  TAG(HTML_HEAD,      "head", 0, 0) \
  TAG(HTML_HEADER,    "header", 0, 0) \
  TAG(HTML_HR,        "hr", 1, 0) \
  TAG(HTML_HTML,      "html", 0, 0) \
  TAG(HTML_I,         "i", 0,  0)  \
  TAG(HTML_IFRAME,    "iframe", 0, 0) \
  TAG(HTML_IMG, "img", 0, 0) \
  TAG(HTML_INPUT, "input", 1, 0) \
  TAG(HTML_INS, "ins", 0, 0) \
  TAG(HTML_KBD, "kbd", 0, 0) \
  TAG(HTML_KEYGEN, "keygen", 0, 0) \
  TAG(HTML_LABEL, "label", 0, 0) \
  TAG(HTML_LEGEND, "legend", 0, 0) \
  TAG(HTML_LI, "li", 0, 0) \
  TAG(HTML_LINK, "link", 1, 0) \
  TAG(HTML_MAIN, "main", 0, 0) \
  TAG(HTML_MAP, "map", 0, 0) \
  TAG(HTML_MARK, "mark", 0, 0) \
  TAG(HTML_MENU, "menu", 0, 0) \
  TAG(HTML_MENUITEM, "menuitem", 0, 0) \
  TAG(HTML_META, "meta", 1, 0) \
  TAG(HTML_METER, "meter", 0, 0) \
  TAG(HTML_NAV, "nav", 0, 0) \
  TAG(HTML_NOFRAMES, "noframes", 0, 0) \
  TAG(HTML_NOSCRIPT, "noscript", 0, 0) \
  TAG(HTML_OBJECT, "object", 0, 0) \
  TAG(HTML_OL, "ol", 0, 0) \
  TAG(HTML_OPTGROUP, "optgroup", 0, 0) \
  TAG(HTML_OPTION, "option", 0, 0) \
  TAG(HTML_OUTPUT, "output", 0, 0) \
  TAG(HTML_P, "p  Defines", 0,  0)  \
  TAG(HTML_PARAM, "param", 0, 0) \
  TAG(HTML_PICTURE, "picture", 0, 0) \
  TAG(HTML_PRE, "pre", 0, 0) \
  TAG(HTML_PROGRESS, "progress", 0, 0) \
  TAG(HTML_RP, "rp", 0, 0) \
  TAG(HTML_RT, "rt", 0, 0) \
  TAG(HTML_RUBY, "ruby", 0, 0) \
  TAG(HTML_SAMP, "samp", 0, 0) \
  TAG(HTML_SCRIPT, "script", 0, 0) \
  TAG(HTML_SECTION, "section", 0, 0) \
  TAG(HTML_SELECT, "select", 0, 0) \
  TAG(HTML_SMALL, "small", 0, 0) \
  TAG(HTML_SOURCE, "source", 0, 0) \
  TAG(HTML_SPAN, "span", 0, 0) \
  TAG(HTML_STRIKE, "strike", 0, 0) \
  TAG(HTML_STRONG, "strong", 0, 0) \
  TAG(HTML_STYLE, "style", 0, 0) \
  TAG(HTML_SUB, "sub", 0, 0) \
  TAG(HTML_SUMMARY, "summary", 0, 0) \
  TAG(HTML_SUP, "sup", 0, 0) \
  TAG(HTML_TABLE, "table", 0, 0) \
  TAG(HTML_TBODY, "tbody", 0, 0) \
  TAG(HTML_TD, "td", 0, 0) \
  TAG(HTML_TEXTAREA, "textarea", 0, 0) \
  TAG(HTML_TFOOT, "tfoot", 0, 0) \
  TAG(HTML_TH, "th", 0, 0) \
  TAG(HTML_THEAD, "thead", 0, 0) \
  TAG(HTML_TIME, "time", 0, 0) \
  TAG(HTML_TITLE, "title", 0, 0) \
  TAG(HTML_TR, "tr", 0, 0) \
  TAG(HTML_TRACK, "track", 0, 0) \
  TAG(HTML_TT, "tt", 0, 0) \
  TAG(HTML_U, "u  Defines", 0,  0)  \
  TAG(HTML_UL, "ul", 0, 0) \
  TAG(HTML_VAR, "var", 0, 0) \
  TAG(HTML_VIDEO, "video", 0, 0) \
  TAG(HTML_WBR, "wbr", 0, 0) \
  TAG(HTML_DOC, "HTML_DOC", 0, 0)

#define TAG(tag, string, self_close, ver) tag,
enum html_tag { HTML_TAG_LIST MAX_HTML_TAGS };
#undef TAG

#define TAG(tag, string, self_close, ver) string,
char *tag_string[] = {HTML_TAG_LIST};
#undef TAG

#define TAG(tag, string, self_close, ver) self_close,
char tag_self_close[] = {HTML_TAG_LIST};
#undef TAG

struct node {
  enum html_tag token;
  bool32 self_close;
  struct node *parent;
  int total_childs;
  int size;
  struct node *childs;
};
void add_node(struct node *root, struct node new_node){
  new_node.parent = root;
  root->childs[root->total_childs] = new_node;
  root->total_childs +=1;
}
void alloc_memory(struct node *node) {
  node->childs = calloc(sizeof(struct node), (unsigned int)INIT_SIZE);
  assert(node->childs);
  node->total_childs = 0;
  node->token = HTML_UNKNOWNTAG;
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
  int *start_token = NULL, /**end_tag = NULL,*/ *end_token = NULL;
  bool32 in_tag = false;
  bool32 is_closing_tag = false;

  struct node root = {0};
  alloc_memory(&root);
  root.token = HTML_DOC;

  struct node *walk = &root;

  while (*interator != '\0') {
    if (*interator == '<') {
      ++interator;
      in_tag = true;
      if (*interator == '/')
      {
        ++interator;
        is_closing_tag = true;
      }
      else
        is_closing_tag = false;

      start_token = interator;
      while (++interator && ((*interator >= 'a' && *interator <= 'z')
            || (*interator >= 'A' && *interator <= 'Z') || *interator == '!'));
      end_token = interator - 1;

      for (int i = 0; i < MAX_HTML_TAGS; i++) {
        if (string_compair_chars(start_token, end_token, tag_string[i]) == 0) {
          if (!is_closing_tag) {
            struct node new_node;
            alloc_memory(&new_node);
            new_node.token = (enum html_tag)i;
            new_node.self_close = tag_self_close[new_node.token];
            add_node(walk, new_node);
            if(!new_node.self_close)
            {

              walk = walk->childs + walk->total_childs - 1;
            }
          } else  walk = walk->parent;
          break;
        }
      }
      start_token = NULL;
      end_token = NULL;
      /* end_tag = NULL; */
    }

    if (in_tag) {
      if (*interator == '>') {
        in_tag = false;
      }
    }
    ++interator;
  }
  print_nodes(&root, 0);
  /* printf("stack size: %d\n", node_stack[0].childs[0].token); */
  fclose(file);
  free(raw_text.data);
  free(root.childs);
  return 0;
}
