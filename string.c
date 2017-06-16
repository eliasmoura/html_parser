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
String string_copy(String* string){
  String str;
  str.capacity = string->capacity;
  str.data = malloc(sizeof(String) * (size_t)str.capacity * sizeof(char));
  assert(string->data != NULL);
  str.size = string->size;
  for(int i=0; i<string->size; i++)
    str.data[i] = string->data[i];
  return str;
}
String string_copy_from_char(char *c){
  String str;
  int count =0;
  while(c[count] != '\0') ++count;
  str.size = 0;
  if(!count) return str;
  str.capacity = count;
  str.data = (int *)malloc(sizeof(String) * (size_t)str.capacity * sizeof(char));
  assert(str.data != NULL);
  for(int i=0; i<str.size; i++)
    str.data[i] = (c[i]);
  return str;
}
void string_append_char(String* string, int c){
  if((string->size+1) > string->capacity)
    string_resize(string, (int)((string->size + 1) * 1.5));
  string->data[string->size] = c;
  string->size += 1;
}
void string_append_chars(String* string, char *c){
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
int  string_compair_with_chars(String* lstring, char* c){
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
int  string_compair_chars(int* lstring, int* end, char* c){
  int char_size = 0, lstring_size = 0;
  while(c[char_size]) ++char_size;
  if(lstring < end) lstring_size = (int)(end-lstring);
  else lstring_size = (int)(lstring-end);
  ++lstring_size;
  assert(lstring_size > 0);
  if(lstring_size != char_size)
    return -1;
  for(int i =0; i< char_size; i++)
    if(lstring[i] != c[i]){
      return -1;
    }
  return 0;
}
int* string_get(String* string, int index)
{
  assert(index >= 0 && index <= MAX_INT);
  if(string->size > 0)
    return &string->data[index];
  return 0;
}
int string_pop(String* string)
{
  assert(string->size > 0 && string->size <= MAX_INT);
  int popedNode = (string->data[string->size]);
  string->data[string->size] = 0;
  string->size -=1;
  return popedNode;
}
void remove_char(String* string, char c){
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
void string_resize(String* string, int size)
{
  assert(size > string->size);
    string->capacity = string->size * 2;
    int* tmp = (int *)realloc(string->data, sizeof(String) * (size_t)string->capacity * sizeof(string->data));
    if(tmp != NULL)
      string->data = tmp;
    for (int i = string->size; i <= string->capacity; i++)
      string->data[i] = '\0';
    assert(tmp != NULL);
}
void string_free(String* string)
{
  free(string->data);
}
