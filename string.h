#include "string.h"
#include <stdlib.h>
#include <assert.h>
#define MAX_INT (int)((unsigned int)(~(int)0)>>1)

void string_init(String *string){
  string->size = 0;
  string->capacity = INITIAL_STRING_CAPACITY;;
  string->content = (char*)malloc(sizeof(String) * string->capacity *sizeof(char));
  for (int i = 0; i <= string->capacity; i++)
    string->content[i] = '\0';
  assert(string->content != NULL);
}
void string_init_size(String *string, int size){
  string->size = size;
  string->capacity = size;
  string->content = calloc(sizeof(String) * string->capacity *sizeof(char));
  for (int i = 0; i <= string->capacity; i++)
    string->content[i] = '\0';
  assert(string->content != NULL);
}
void string_append(String* string, String* value){
  if((string->size+value->size) > string->capacity)
    string_resize(string, string->size + value->size *1.5);
  for(int i =0; i< value->size;i++)
  string->content[string->size+i] = value->content[i];
  string->size += 1;
}
String string_copy(String* string){
  String tmp;
  tmp.capacity = string->capacity;
  string->content = malloc(sizeof(String) * tmp.capacity *sizeof(char));
  assert(string->content != NULL);
  tmp.size = string->size;
  for(int i=0; i<string->size; i++)
    tmp.content[i] = (string->content[i]);
  return tmp;
}
String string_copy_from_char(char *c){
  String tmp;
  int count =0;
  while(c[count] != '\0') ++count;
  tmp.size = 0;
  tmp.capacity = count;
  tmp.content = (char *)malloc(sizeof(String) * tmp.capacity *sizeof(char));
  assert(tmp.content != NULL);
  for(int i=0; i<tmp.size; i++)
    tmp.content[i] = (tmp.content[i]);
  return tmp;
}
void string_append_char(String* string, char c){
  if((string->size+1) > string->capacity)
    string_resize(string, (string->size + 1) *1.5);
  string->content[string->size] = c;
  string->size += 1;
}
void string_append_chars(String* string, char *c){
  int count =0;
  while(c[count] != '\0') ++count;
  if((string->size+count) > string->capacity)
    string_resize(string, (string->size + count) *1.5);
  for (int i =0; i < count; i++)
    string->content[string->size++] = c[i];
}
int  string_compair(String* lstring, String* rstring){
  int size = lstring->size > rstring->size ? rstring->size:lstring->size;
  int equality = 0;
  for(int i =0; i< size;i++)
    if(lstring->content[i] != rstring->content[i]){
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
    if(lstring->content[i] != c[i]){
      equality = -1;
      break;
    }
  return equality;
}
int  string_compair_chars(char* lstring, char* end, char* c){
  int char_size = 0, lstring_size = 0;
  while(c[char_size] != '\0') char_size++;
  while(&lstring[lstring_size] != end) lstring_size++;
  int equality = 0;
  if(lstring_size != char_size)
    return -1;
  for(int i =0; i< char_size; i++)
    if(lstring[i] != c[i]){
      equality = -1;
      break;
    }
  return equality;
}
char* string_get(String* string, int index)
{
  assert(index >= 0 && index <= MAX_INT);
  if(string->size > 0)
    return &string->content[index];
  return 0;
}
char  string_pop(String* string)
{
  assert(string->size > 0 && string->size <= MAX_INT);
  char popedNode = (string->content[string->size]);
  string->content[string->size] = 0;
  string->size -=1;
  return popedNode;
}
void remove_char(String* string, char c){
  String tmp;
  tmp.size = string->size;
  string_init_size(&tmp, string->size);
  int tmpIndex = 0;
  for(int i =0; i < string->size; i++){
    if(string->content[i] == c)
      continue;
    tmp.content[tmpIndex] = string->content[tmpIndex];
    ++tmpIndex;
  }

}
void string_resize(String* string, int size)
{
  assert(size > string->size);
    string->capacity = string->size * 2;
    char* tmp = (char *)realloc(string->content, sizeof(String) * string->capacity *sizeof(char));
    if(tmp != NULL)
      string->content = tmp;
    for (int i = string->size; i <= string->capacity; i++)
      string->content[i] = '\0';
    assert(tmp != NULL);
}
void string_free(String* string)
{
  free(string->content);
}
