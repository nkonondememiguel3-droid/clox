#include "value.h"
#include "memory.h"
#include <stdio.h>

void init_value_array(__value_array_t__ *array) {

  array->capacity = 0;
  array->count = 0;
  array->values = NULL;
}

void write_value_array(__value_array_t__ *array, __value_t__ value) {

  if (array->capacity < array->count + 1) {
    int old_capacity = array->capacity;

    array->capacity = GROW_CAPACITY(old_capacity);
    array->values =
        GROW_ARRAY(__value_t__, array->values, old_capacity, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void free_value_array(__value_array_t__ *array) {
  FREE_ARRAY(__value_t__, array->values, array->capacity);
  init_value_array(array);
}

void print_value(__value_t__ value) {

  printf("%g", value);
}
