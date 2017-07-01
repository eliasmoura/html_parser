#include "string.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#define MAX_INT (int)((unsigned int)(~(int)0)>>1)

void string_init(String *string){
  string->size = 0;
  string->capacity = 100;
  string->data = (int*)malloc((size_t)(size_t)(string->capacity) * sizeof(int));
  assert(string->data != NULL);
}
void string_init_size(String *string, int size){
  string->size = size;
  string->capacity = size;
  string->data = (int *)malloc(sizeof(String) * (size_t)string->capacity *sizeof(string->size));
  for (int i = 0; i <= string->capacity; i++)
    string->data[i] = '\0';
  assert(string->data != NULL);
}
void string_append(String* string, String* value){
  if((string->size+value->size) > string->capacity)
    string_resize(string, string->size + (int)(value->size * 1.5));
  for(int i =0; i< value->size;i++)
  string->data[string->size+i] = value->data[i];
  string->size += 1;
}
void string_copy(wchar_t *start, wchar_t *end, String *dest){
  int size = (int)(end-start)+1;
  if(dest->capacity < size)
    dest->data = malloc(sizeof(wchar_t) * (size_t)(size+1));
  assert(dest->data != NULL);
  dest->size = size;
  for(size_t i = 0; start<=end; ++start)
    dest->data[i++] = *start;
  dest->data[dest->size] = '\0';
}
String string_copy_from_char(wchar_t *c){
  String str;
  int count =0;
  while(c[count] != '\0') ++count;
  str.size = 0;
  if(!count) return str;
  str.capacity = count;
  str.data = (wchar_t *)malloc(sizeof(String) * (size_t)str.capacity * sizeof(char));
  assert(str.data != NULL);
  for(int i=0; i<str.size; i++)
    str.data[i] = (c[i]);
  return str;
}
void string_append_char(String* string, wchar_t c){
  if(string->size >= string->capacity)
    string_resize(string, (int)((string->size + 1) * 1.5));
  string->data[string->size] = c;
  string->size += 1;
}
void string_append_chars(String* string, wchar_t *c){
  int count =0;
  while(c[count] != '\0') ++count;
  if((string->size+count) > string->capacity)
    string_resize(string, (int)((string->size + count) * 1.5));
  for (int i =0; i < count; i++)
    string->data[string->size++] = c[i];
}
int  string_compair(String* lstring, String* rstring){
  int size = lstring->size > rstring->size ? rstring->size:lstring->size;
  int equality = 0;
  for(int i =0; i< size;i++)
    if(lstring->data[i] != rstring->data[i]){
      equality = -1;
      break;
    }
  return equality;
}
int  string_compair_with_chars(String* lstring, wchar_t * c){
  int char_size = 0;
  while(c[char_size] != '\0') char_size++;
  int size = lstring->size > char_size ? char_size:lstring->size;
  int equality = 0;
  for(int i =0; i< size;i++)
    if(lstring->data[i] != c[i]){
      equality = -1;
      break;
    }
  return equality;
}
int  string_compair_chars(wchar_t * lstring, wchar_t * end, wchar_t * c){
  size_t char_size = wcslen(c), lstring_size = 0;
  while(c[char_size]) ++char_size;
  if(lstring < end) lstring_size = (size_t)(end-lstring);
  else lstring_size = (size_t)(lstring-end);
  ++lstring_size;
  assert(lstring_size > 0);
  if(lstring_size != char_size)
    return -1;
  for(size_t i =0; i< char_size; i++)
    if(lstring[i] != c[i]){
      return -1;
    }
  return 0;
}
int* string_get(String* string, int index) {
  assert(index >= 0 && index <= MAX_INT);
  if(string->size > 0)
    return &string->data[index];
  return 0;
}
int string_pop(String* string) {
  assert(string->size > 0 && string->size <= MAX_INT);
  int popedNode = (string->data[string->size]);
  string->data[string->size] = 0;
  string->size -=1;
  return popedNode;
}
void remove_char(String* string, wchar_t  c){
  String tmp;
  tmp.size = string->size;
  string_init_size(&tmp, string->size);
  int tmpIndex = 0;
  for(int i =0; i < string->size; i++){
    if(string->data[i] != c)
      continue;
    tmp.data[tmpIndex] = string->data[tmpIndex];
    ++tmpIndex;
  }
  free(string->data);
  string->data = tmp.data;
}
void string_resize(String* string, int size) {
  assert(size > string->size);
    string->capacity = string->size * 2;
    int* tmp = (int *)realloc(string->data, sizeof(String) * (size_t)string->capacity * sizeof(string->data));
    if(tmp != NULL)
      string->data = tmp;
    for (int i = string->size; i <= string->capacity; i++)
      string->data[i] = '\0';
    assert(tmp != NULL);
}
void free_string(String* string) {
  free(string->data);
  string->size = 0;
  string->capacity = 0;
}
