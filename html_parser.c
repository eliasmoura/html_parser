#include "string.h"
#include <assert.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef float real32;
typedef double real64;

#define false 0
#define true 1
#define INIT_SIZE 1000

// TAG(enum, string, self_close, ver)
// NOTE: ver is for checking if the tag is depracated(0)/HTML5 only(1)/both(2)
// NOTE: self_close: 0 has a </> close tag, 1 does not
#define HTML_TAG_LIST                                                          \
  TAG(HTML_UNKNOWNTAG, L"HTNL_UNKNOWNTAG", 1, 0)                               \
  TAG(HTML_DOCTYPE, L"!DOCTYPE", 1, 1)                                         \
  TAG(HTML_COMMENT, L"!--", 1, 2)                                              \
  TAG(HTML_A, L"a", 0, 2)                                                      \
  TAG(HTML_ABBR, L"abbr", 0, 2)                                                \
  TAG(HTML_ACRONYM, L"acronym", 0, 0)                                          \
  TAG(HTML_ADDRESS, L"address", 0, 2)                                          \
  TAG(HTML_APPLET, L"applet", 0, 0)                                            \
  TAG(HTML_AREA, L"area", 0, 2)                                                \
  TAG(HTML_ARTICLE, L"article", 0, 1)                                          \
  TAG(HTML_ASIDE, L"aside", 0, 1)                                              \
  TAG(HTML_AUDIO, L"audio", 0, 1)                                              \
  TAG(HTML_B, L"b", 0, 2)                                                      \
  TAG(HTML_BASE, L"base", 0, 2)                                                \
  TAG(HTML_BASEFONT, L"basefont", 0, 0)                                        \
  TAG(HTML_BDI, L"bdi", 0, 1)                                                  \
  TAG(HTML_BDO, L"bdo", 0, 0)                                                  \
  TAG(HTML_BIG, L"big", 0, 0)                                                  \
  TAG(HTML_BLOCKQUOTE, L"blockquote", 0, 0)                                    \
  TAG(HTML_BODY, L"body", 0, 0)                                                \
  TAG(HTML_BR, L"br", 1, 0)                                                    \
  TAG(HTML_BUTTON, L"button", 0, 0)                                            \
  TAG(HTML_CANVAS, L"canvas", 0, 0)                                            \
  TAG(HTML_CAPTION, L"caption", 0, 0)                                          \
  TAG(HTML_CENTER, L"center", 0, 0)                                            \
  TAG(HTML_CITE, L"cite", 0, 0)                                                \
  TAG(HTML_CODE, L"code", 0, 0)                                                \
  TAG(HTML_COL, L"col", 0, 0)                                                  \
  TAG(HTML_COLGROUP, L"colgroup", 0, 0)                                        \
  TAG(HTML_DATALIST, L"datalist", 0, 0)                                        \
  TAG(HTML_DD, L"dd", 0, 0)                                                    \
  TAG(HTML_DEL, L"del", 0, 0)                                                  \
  TAG(HTML_DETAILS, L"details", 0, 0)                                          \
  TAG(HTML_DFN, L"dfn", 0, 0)                                                  \
  TAG(HTML_DIALOG, L"dialog", 0, 0)                                            \
  TAG(HTML_DIR, L"dir", 0, 0)                                                  \
  TAG(HTML_DIV, L"div", 0, 0)                                                  \
  TAG(HTML_DL, L"dl", 0, 0)                                                    \
  TAG(HTML_DT, L"dt", 0, 0)                                                    \
  TAG(HTML_EM, L"em", 0, 0)                                                    \
  TAG(HTML_EMBED, L"embed", 0, 0)                                              \
  TAG(HTML_FIELDSET, L"fieldset", 0, 0)                                        \
  TAG(HTML_FIGCAPTION, L"figcaption", 0, 0)                                    \
  TAG(HTML_FIGURE, L"figure", 0, 0)                                            \
  TAG(HTML_FONT, L"font", 0, 0)                                                \
  TAG(HTML_FOOTER, L"footer", 0, 0)                                            \
  TAG(HTML_FORM, L"form", 0, 0)                                                \
  TAG(HTML_FRAME, L"frame", 0, 0)                                              \
  TAG(HTML_FRAMESET, L"frameset", 0, 0)                                        \
  TAG(HTML_H1, L"h1", 0, 0)                                                    \
  TAG(HTML_HEAD, L"head", 0, 0)                                                \
  TAG(HTML_HEADER, L"header", 0, 0)                                            \
  TAG(HTML_HR, L"hr", 1, 0)                                                    \
  TAG(HTML_HTML, L"html", 0, 0)                                                \
  TAG(HTML_I, L"i", 0, 0)                                                      \
  TAG(HTML_IFRAME, L"iframe", 0, 0)                                            \
  TAG(HTML_IMG, L"img", 0, 0)                                                  \
  TAG(HTML_INPUT, L"input", 1, 0)                                              \
  TAG(HTML_INS, L"ins", 0, 0)                                                  \
  TAG(HTML_KBD, L"kbd", 0, 0)                                                  \
  TAG(HTML_KEYGEN, L"keygen", 0, 0)                                            \
  TAG(HTML_LABEL, L"label", 0, 0)                                              \
  TAG(HTML_LEGEND, L"legend", 0, 0)                                            \
  TAG(HTML_LI, L"li", 0, 0)                                                    \
  TAG(HTML_LINK, L"link", 1, 0)                                                \
  TAG(HTML_MAIN, L"main", 0, 0)                                                \
  TAG(HTML_MAP, L"map", 0, 0)                                                  \
  TAG(HTML_MARK, L"mark", 0, 0)                                                \
  TAG(HTML_MENU, L"menu", 0, 0)                                                \
  TAG(HTML_MENUITEM, L"menuitem", 0, 0)                                        \
  TAG(HTML_META, L"meta", 1, 0)                                                \
  TAG(HTML_METER, L"meter", 0, 0)                                              \
  TAG(HTML_NAV, L"nav", 0, 0)                                                  \
  TAG(HTML_NOFRAMES, L"noframes", 0, 0)                                        \
  TAG(HTML_NOSCRIPT, L"noscript", 0, 0)                                        \
  TAG(HTML_OBJECT, L"object", 0, 0)                                            \
  TAG(HTML_OL, L"ol", 0, 0)                                                    \
  TAG(HTML_OPTGROUP, L"optgroup", 0, 0)                                        \
  TAG(HTML_OPTION, L"option", 0, 0)                                            \
  TAG(HTML_OUTPUT, L"output", 0, 0)                                            \
  TAG(HTML_P, L"p", 0, 0)                                                      \
  TAG(HTML_PARAM, L"param", 0, 0)                                              \
  TAG(HTML_PICTURE, L"picture", 0, 0)                                          \
  TAG(HTML_PRE, L"pre", 0, 0)                                                  \
  TAG(HTML_PROGRESS, L"progress", 0, 0)                                        \
  TAG(HTML_RP, L"rp", 0, 0)                                                    \
  TAG(HTML_RT, L"rt", 0, 0)                                                    \
  TAG(HTML_RUBY, L"ruby", 0, 0)                                                \
  TAG(HTML_SAMP, L"samp", 0, 0)                                                \
  TAG(HTML_SCRIPT, L"script", 0, 0)                                            \
  TAG(HTML_SECTION, L"section", 0, 0)                                          \
  TAG(HTML_SELECT, L"select", 0, 0)                                            \
  TAG(HTML_SMALL, L"small", 0, 0)                                              \
  TAG(HTML_SOURCE, L"source", 0, 0)                                            \
  TAG(HTML_SPAN, L"span", 0, 0)                                                \
  TAG(HTML_STRIKE, L"strike", 0, 0)                                            \
  TAG(HTML_STRONG, L"strong", 0, 0)                                            \
  TAG(HTML_STYLE, L"style", 0, 0)                                              \
  TAG(HTML_SUB, L"sub", 0, 0)                                                  \
  TAG(HTML_SUMMARY, L"summary", 0, 0)                                          \
  TAG(HTML_SUP, L"sup", 0, 0)                                                  \
  TAG(HTML_TABLE, L"table", 0, 0)                                              \
  TAG(HTML_TBODY, L"tbody", 0, 0)                                              \
  TAG(HTML_TD, L"td", 0, 0)                                                    \
  TAG(HTML_TEXTAREA, L"textarea", 0, 0)                                        \
  TAG(HTML_TFOOT, L"tfoot", 0, 0)                                              \
  TAG(HTML_TH, L"th", 0, 0)                                                    \
  TAG(HTML_THEAD, L"thead", 0, 0)                                              \
  TAG(HTML_TIME, L"time", 0, 0)                                                \
  TAG(HTML_TITLE, L"title", 0, 0)                                              \
  TAG(HTML_TR, L"tr", 0, 0)                                                    \
  TAG(HTML_TRACK, L"track", 0, 0)                                              \
  TAG(HTML_TT, L"tt", 0, 0)                                                    \
  TAG(HTML_U, L"u", 0, 0)                                                      \
  TAG(HTML_UL, L"ul", 0, 0)                                                    \
  TAG(HTML_VAR, L"var", 0, 0)                                                  \
  TAG(HTML_VIDEO, L"video", 0, 0)                                              \
  TAG(HTML_WBR, L"wbr", 0, 0)                                                  \
  TAG(HTML_TEXT, L"text", 1, 0)                                                \
  TAG(HTML_DOC, L"HTML_DOC", 1, 0)

#define TAG(tag, string, self_close, ver) tag,
enum html_tag { HTML_TAG_LIST MAX_HTML_TAGS };
#undef TAG

#define TAG(tag, string, self_close, ver) string,
wchar_t *tag_string[] = {HTML_TAG_LIST};
#undef TAG

#define TAG(tag, string, self_close, ver) self_close,
uint8_t tag_self_close[] = {HTML_TAG_LIST};
#undef TAG
struct attribute {
  int id;
  String name;
  String value;
};

struct node {
  enum html_tag token;
  bool32 self_close;
  struct node *parent;
  struct attribute *attr;
  int attr_size;
  int attr_cap;
  int cap;
  int size;
  struct node *childs;
};
void node_init(struct node *root, enum html_tag tag) {
  root->token = tag;
  root->self_close = tag_self_close[root->token];
  if (!root->self_close || root->token == HTML_DOC) {
    root->childs = calloc(sizeof(struct node), (unsigned int)INIT_SIZE);
    assert(root->childs);
  } else
    root->childs = NULL;
  root->cap = INIT_SIZE;
  root->size = 0;
  root->attr = NULL;
  root->attr_size = 0;
  root->attr_cap = 0;
}
void add_node(struct node *root, enum html_tag new_node) {
  struct node tmp;
  node_init(&tmp, new_node);
  tmp.parent = root;
  root->childs[root->size] = tmp;
  ++root->size;
}
void add_attribute(struct node *root, struct attribute *attr) {
  if (!root->attr) {
    root->attr = calloc(sizeof(struct attribute), (unsigned int)root->cap);
    root->attr_size = 0;
    root->attr_cap = 10;
  }
  if (root->attr_size >= root->attr_cap) {
    root->attr_cap = 10 * 2;
    struct attribute *tmp = root->attr;
    root->attr = calloc(sizeof(struct attribute), (unsigned int)root->cap);
    for (int i = 0; i < root->size; ++i)
      root->attr[i] = tmp[i];
    free(tmp);
  }
  root->attr[root->attr_size].name = attr->name;
  root->attr[root->attr_size].value = attr->value;
  root->attr_size += 1;
}
void free_attr(struct attribute *attr) {
  if (attr) {
    free_string(&attr->name);
    free_string(&attr->value);
  }
}
void free_nodes(struct node *root) {
  while (root) {
    if (root->size) {
      root = root->childs + (--(root->size));
      continue;
    } else
      free(root->childs);

    for (int i = 0; i < root->attr_size; ++i) {
      assert(root->attr && &(root->attr[i]));
      free_attr(&(root->attr[i]));
    }
    free(root->attr);
    root->attr_size = 0;
    root = root->parent;
  }
  return;
  /*if(root)
   {
    if(root->attr_size){
      for(int i = 0; i < root->attr_size; ++i)
      {
        free_attr(&root->attr[i]);
        --root->attr_size;
      }
      --root->attr_size;
      free(root->attr);
    }
    int size = root->size;
    for(int i = 0; i < size; ++i){
      free_nodes(root->childs + i);
      root->size -= 1;
    }
    if(root->parent && root->parent->childs[0].token == root->token)
      free(root->parent->childs);
  }*/
}
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
int main(int argc, char **argv) {
  setlocale(LC_CTYPE, NULL);
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
  wint_t element;
  String raw_text;
  string_init(&raw_text);
  while ((element = fgetwc(file)) != '\0' && element != WEOF) {
    string_append_char(&raw_text, (wchar_t)element);
  }

  wchar_t *interator = raw_text.data;
  wchar_t *start_token = NULL, /**end_tag = NULL,*/ *end_token = NULL;
  bool32 in_tag = false;
  bool32 is_closing_tag = false;

  struct node root = {0};
  node_init(&root, HTML_DOC);
  root.self_close = 1;

  struct node *walk = &root;

  while (*interator != '\0') {
    if (*interator == '<') {
      ++interator;
      in_tag = true;
      if (*interator == '/') {
        ++interator;
        is_closing_tag = true;
      } else
        is_closing_tag = false;

      start_token = interator;
      while (++interator &&
             ((*interator >= 'a' && *interator <= 'z') ||
              (*interator >= 'A' && *interator <= 'Z') || *interator == '-'))
        ;
      end_token = interator - 1;

      for (int i = 0; i < MAX_HTML_TAGS; i++) {
        if (string_compair_chars(start_token, end_token,
                                 (wchar_t *)tag_string[i]) == 0) {
          if (!is_closing_tag) {
            add_node(walk, (enum html_tag)i);
            // XXX: I would like to use something like this, this makes things
            // harder, howerver.
            /* if(!walk->childs[walk->size-1].self_close) */
            /* { */
            /*   walk = walk->childs + walk->size - 1; */
            /* } */
            walk = walk->childs + (walk->size - 1);
          } else if (walk->parent)
            walk = walk->parent;
          break;
        }
      }
      start_token = NULL;
      end_token = NULL;
    }

    if (in_tag) {
      while (*interator != '>') {
        if ((*interator == ' ' || *interator == '/') &&
            walk->token != HTML_COMMENT) {
          ++interator;
          continue;
        }

        start_token = interator;
        if (walk->token == HTML_COMMENT) {
          while (++interator && (*interator != '-' || *(interator + 1) != '-' ||
                                 *(interator + 2) != '>'))
            ;
          end_token = interator - 1;
          struct attribute attr = {0};
          string_init(&attr.name);
          memcpy(&attr.name, tag_string[HTML_TEXT],
                 sizeof(tag_string[HTML_TEXT]) >
                         (long unsigned int)attr.name.size
                     ? (long unsigned)attr.name.size
                     : sizeof(tag_string[HTML_TEXT]));
          string_copy(start_token, end_token, &attr.value);
          add_attribute(walk, &attr);
          interator = interator + 2;
          continue;
        }
        while (++interator &&
               ((*interator >= 'a' && *interator <= 'z') ||
                (*interator >= 'A' && *interator <= 'Z') || *interator == '-'))
          ;
        end_token = interator - 1;

        struct attribute attr = {0};
        string_init(&attr.name);
        string_copy(start_token, end_token, &attr.name);

        if (*interator != '=') {
          add_attribute(walk, &attr);
          continue;
        }
        ++interator;
        if (*interator == '"')
          ++interator;

        start_token = interator;
        while (*interator != '"' && *interator != '>' && *interator != '<')
          ++interator;
        end_token = interator - 1;
        string_init(&attr.value);
        string_copy(start_token, end_token, &attr.value);

        add_attribute(walk, &attr);
        if (*interator != '>')
          ++interator;
      }
      if (tag_self_close[walk->token] && walk->parent)
        walk = walk->parent;
      start_token = NULL;
      end_token = NULL;
      in_tag = false;
    } else {
      while (walk->token != HTML_PRE &&
             (*interator == ' ' || *interator == '\n' || *interator == '\r' ||
              *interator == '\t'))
        ++interator;
      if (*interator == '<' || (is_closing_tag && (walk->token == HTML_DOC ||
                                                   walk->token == HTML_HTML)))
        continue;
      if (walk->token == HTML_SCRIPT) {
        // XXX I need to do more checks under SRIPT tags.
        while (*interator == ' ' || *interator == '\n' || *interator == '\r' ||
               *interator == '\t')
          ++interator;
        if (*interator == '<' || (is_closing_tag && (walk->token == HTML_DOC ||
                                                     walk->token == HTML_HTML)))
          continue;
        start_token = interator;
        bool32 under_squotes = false;
        bool32 under_dquotes = false;
        while (*interator != '<' || (under_dquotes || under_squotes)) {
          if (*interator == '"' && !under_squotes)
            under_dquotes = !under_dquotes;
          if (*interator == '\'' && !under_dquotes)
            under_squotes = !under_squotes;
          ++interator;
        }
        wchar_t *tmp = interator;
        while (--tmp && *tmp != '>' &&
               (*tmp == ' ' || *tmp == '\n' || *tmp == '\r' || *tmp == '\t'))
          ;
        end_token = interator - 1;

        struct attribute attr = {0};
        wchar_t str[] = L"text";
        string_copy(str, &str[(sizeof(str) / sizeof(wchar_t)) - 1], &attr.name);
        string_copy(start_token, end_token, &attr.value);
        add_node(walk, HTML_TEXT);
        add_attribute(&walk->childs[walk->size - 1], &attr);

        continue;
      }

      start_token = interator;
      while (*interator != '<' && *interator)
        ++interator;
      end_token = interator - 1;

      struct attribute attr = {0};
      wchar_t str[] = L"text";
      string_copy(str, &str[(sizeof(str) / sizeof(wchar_t)) - 1], &attr.name);
      string_copy(start_token, end_token, &attr.value);
      add_node(walk, HTML_TEXT);
      add_attribute(walk->childs + walk->size - 1, &attr);

      continue;
    }
    ++interator;
  }
  print_nodes(&root, 0);
  fclose(file);
  free(raw_text.data);
  free_nodes(&root);
  return 0;
}
