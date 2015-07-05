#ifndef _ARRAY_

#define INITIAL_ARRAY_CAPACITY 100

typedef struct Array {
  int size;
  int capacity;
  int* data;
};

void array_init(Array* array);
void array_append(Array* array, int value);
int array_get(Array* array, int index);
void array_set(Array* array, int index, int value);
void array_double_capacity_if_full(Array* array);
void array_free(Array* array);


#endif
