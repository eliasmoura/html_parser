#include "array.h"
#include <assert.h>

void array_init(Array *array){
  array->size = 0;
  array->capacity = INITIAL_ARRAY_CAPACITY;;
  array->data = malloc(sizeof(int) * array->capacity;
      }
void array_appen
