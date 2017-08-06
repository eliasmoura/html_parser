#include "string.h"
#include "html_parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void node_init(struct node *root, enum html_tag tag) {
  root->token = tag;
  root->self_close = tag_self_close[root->token];
  if (!root->self_close) {
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
  if (!root->attr_cap) {
    root->attr = calloc(sizeof(struct attribute), (unsigned int)root->cap);
    root->attr_size = 0;
    root->attr_cap = 10;
  } else if (root->attr_size >= root->attr_cap) {
    int attr_cap = root->attr_size * 2;
    struct attribute *tmp =
        realloc(root->attr, sizeof(struct attribute) * (unsigned int)attr_cap);
    if (tmp != NULL) {
      root->attr_cap = (int)attr_cap;
      root->attr = (struct attribute *)tmp;
      for (int i = root->attr_size; i < root->attr_cap; ++i)
        root->attr[i] = (struct attribute){0};
    }
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

void parse(struct node *node, wchar_t *text) {
  wchar_t *interator = text;
  wchar_t *start_token = NULL, *end_token = NULL;
  bool32 in_tag = false;
  bool32 is_closing_tag = false;
  struct node *walk = node;
    while (*interator != '\0' ) {
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
}
