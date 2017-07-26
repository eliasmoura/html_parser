#include <assert.h>
#include <stdlib.h>
#include <wchar.h>
#define MAX_INT (int)((unsigned int)(~(int)0) >> 1)

typedef struct _string {
  int size;
  int capacity;
  wchar_t *data;
} String;

void string_init(String *string);
void string_init_size(String *string, int size);
void string_append(String *string, String *value);
void string_copy(wchar_t *start, wchar_t *end, String *dest);
String string_copy_from_char(wchar_t *c);
void string_append_char(String *string, wchar_t c);
void string_append_chars(String *string, wchar_t *c);
int string_compair(String *lstring, String *rstring);
int string_compair_with_chars(String *lstring, wchar_t *c);
int string_compair_chars(wchar_t *lstring, wchar_t *end, wchar_t *c);
int *string_get(String *string, int index);
int string_pop(String *string);
void remove_char(String *string, wchar_t c);
void string_resize(String *string, int size);
void free_string(String *string);
