#include <stdlib.h>
#include <assert.h>
#define MAX_INT (int)((unsigned int)(~(int)0)>>1)

typedef struct _string {
  int size;
  int capacity;
  int *data;
} String;

void string_init(String *string);
void string_init_size(String *string, int size);
void string_append(String* string, String* value);
String string_copy(String* string);
String string_copy_from_char(char *c);
void string_append_char(String* string, int c);
void string_append_chars(String* string, char *c);
int  string_compair(String* lstring, String* rstring);
int  string_compair_with_chars(String* lstring, char* c);
int  string_compair_chars(int* lstring, int* end, char* c);
int* string_get(String* string, int index) ;
int  string_pop(String* string) ;
void remove_char(String* string, char c);
void string_resize(String* string, int size) ;
void string_free(String* string) ;
