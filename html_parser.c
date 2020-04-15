#include "html_parser.h"
#include "string.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void node_init(struct node *root, enum html_tag tag) {
  root->token = tag;
  root->self_close = tag_self_close[root->token];
  root->childs = NULL;
  root->cap = INIT_SIZE;
  root->size = 0;
  root->attr = NULL;
  root->attr_size = 0;
  root->attr_cap = 0;
}

void add_node(struct node *root, enum html_tag new_node) {
  if (!root->childs) {
    int size = INIT_SIZE;
    if (root->size >= INIT_SIZE)
      size = root->size * 2;
    root->childs = calloc(sizeof(struct node), (unsigned int)size);
    assert(root->childs);
  }
  node_init(&root->childs[root->size], new_node);
  root->childs[root->size].parent = root;
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
}

struct node **search(struct node *root, wchar_t *str, int *size) {
  struct node *srch = calloc(sizeof(srch), 10);
  int index = 0;
  wchar_t *start = str, *end = NULL;
  struct node **result = 0;
  while (*str != '\0') {
    while ((*str == ' ' || *str == '\n' || *str == '\r' || *str == '\t') &&
           *str != '\0')
      ++str;
    if (*str == '.') {
      start = ++str;
      while (*str != ' ' && *str != '\n' && *str != '\r' && *str != '\t' &&
             *str != '\0')
        ++str;
      end = str - 1;
      srch[index].token = get_token(start, end);
    } else if (*str == '#') {
      start = ++str;
      while (*str != ' ' && *str != '\n' && *str != '\r' && *str != '\t' &&
             *str != '\0')
        ++str;
      end = str - 1;
      srch[index].token = get_token(start, end);
    } else {
      start = str;
      while (*str != ' ' && *str != '\n' && *str != '\r' && *str != '\t' &&
             *str != '\0')
        ++str;
      end = str - 1;
      srch[index].token = get_token(start, end);
    }
    // > , [input] ![text] !([password] || [email])
    ++index;
  }

  result = get_nodes(root, srch, index, size);
  free(srch);
  if (result)
    return result;
  return 0;
}

struct node *next_node(struct node *node) {
  struct node *prev = node;
  struct node *next = node;
  while (next) {
    if (next->childs != NULL) {
      if (next == prev->parent) {
        if (prev != next->childs + (next->size - 1)) { // not the last child
          next = ++prev;
          break;
        } else { // if(next != next->childs+next->size-1)
          prev = next;
          next = next->parent;
        }
      } else { // if(next == prev->parent)
        next = next->childs;
        break;
      }
    } else { // if(next->childs != NULL)
      prev = next;
      next = next->parent;
    }
  }
  return next;
}

bool is_in_node(struct node *root, struct node *child) {
  bool result = false;
  while (child && child != root) {
    if (root == child) {
      result = true;
      break;
    }
    child = child->parent;
  }
  return result;
}

struct node **get_nodes(struct node *root, struct search *srch, int srch_size,
                        int *size) {
  struct node *it;
  int l_index = 0;
  struct node **list;
  list = calloc((size_t)l_index + 10, sizeof(list[0]));
  it = root;
  struct node *base = it;

  while (it) {
    int s_index = 0;
    bool is_node = false;
    for (; s_index < srch_size && it; ++s_index) {
      is_node = false;
      if (srch[s_index].node.token != HTML_UNKNOWNTAG) {
        while (it) {
          if (it->token == srch[s_index].node.token) {
            if (is_in_node(base, it))
              is_node = true;
            break;
          } else {
            it = next_node(it);
          }
        }
      }

      if (it && srch[s_index].node.attr) {
        for (int index = 0; index < srch[s_index].node.attr_size; index++) {
          for (int attr_index = 0; attr_index < it->attr_size; attr_index++) {
            if (srch[s_index].node.attr[index].id == it->attr[attr_index].id) {
              if (string_compair(&srch[s_index].node.attr[index].value,
                                 &it->attr[attr_index].value) == 0) {
                is_node = true;
              } else {
                is_node = false;
                index = srch[s_index].node.attr_size;
                break;
              }
            }
          }
        }
      } // if(it && srch[s_index].attr)
      if (is_node)
        base = it;
      if (s_index < srch_size - 1)
        it = next_node(it);
    } // for(; s_index < srch_size; ++s_index)
    if (is_node && s_index == srch_size) {
      list[l_index++] = it;
      it = next_node(it);
    }
  }
  *size = l_index;
  return list;
}

enum html_tag get_token(wchar_t *start, wchar_t *end) {
  for (int i = 0; i < MAX_HTML_TAGS; i++) {
    if (string_compair_chars(start, end, (wchar_t *)tag_string[i]) == 0) {
      return (enum html_tag)i;
    }
  }
  return HTML_UNKNOWNTAG;
}

void parse(struct node *node, wchar_t *text) {
  wchar_t *interator = text;
  wchar_t *start_token = NULL, *end_token = NULL;
  bool32 in_tag = false;
  bool32 is_closing_tag = false;
  struct node *walk = node;
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
              (*interator >= 'A' && *interator <= 'Z') ||
              (*interator >= '0' && *interator <= '9') || *interator == '-'))
        ;
      end_token = interator - 1;

      if (!is_closing_tag) {
        add_node(walk, get_token(start_token, end_token));
        // XXX: I would like to use something like this, this makes things
        // harder, howerver.
        // if(!walk->childs[walk->size-1].self_close)
        // {
        //   walk = walk->childs + walk->size - 1;
        // }
        walk = walk->childs + (walk->size - 1);
      } else if (walk->parent) {
        walk = walk->parent;
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
