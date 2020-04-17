#ifndef _HTML_PARSER_H
#define _HTML_PARSER_H

#include "string.h"
#include <stdint.h>
#include <wchar.h>

typedef uint32_t bool32;

#define false 0
#define true 1
#define INIT_SIZE 1000

// TAG(enum, string, self_close, ver)
// NOTE: ver is for checking if the tag is depracated(0)/HTML5 only(1)/both(2)
// NOTE: self_close: 0 <tag> has a </tag> close tag, 1 does not
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
  TAG(HTML_DOC, L"HTML_DOC", 0, 0)

#define TAG(tag, string, self_close, ver) tag,
enum html_tag { HTML_TAG_LIST MAX_HTML_TAGS };
#undef TAG

#define TAG(tag, string, self_close, ver) string,
wchar_t *tag_string[] = {HTML_TAG_LIST};
#undef TAG

#define TAG(tag, string, self_close, ver) self_close,
uint8_t tag_self_close[] = {HTML_TAG_LIST};
#undef TAG

enum srch_type { TAG_ATTR, HTML_TAG, TAG_CLASS, TAG_ID, INPUT_TYPE };

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

struct search_element {
    unsigned int size;
    unsigned int cap;
    struct {
      struct node node;
      bool hard_depend;
    } *elemets;
};

struct search {
  struct search_element search_element;
  unsigned int size;
  unsigned int cap;
  struct node **result;
  String str;
};

enum html_tag token_get(wchar_t *start, wchar_t *end);
void parse(struct node *node, wchar_t *text);

void node_init(struct node *root, enum html_tag tag);
void node_add(struct node *root, enum html_tag new_node);
void node_free(struct node *root);

unsigned long attribute_get(wchar_t *start, struct attribute *attr);
void attribute_add(struct node *root, struct attribute *attr);
void attribute_free(struct attribute *attr);

/* Search for the nodes by css matching system. */
void search_free(struct search *search);
void search(struct node *root, struct search *srch, wchar_t *str);
void node_get(struct node *root, struct search *srch);
int get_element_by_id(struct node *src, struct node *dest, wchar_t *str);
#endif // _HTML_PARSER_H
