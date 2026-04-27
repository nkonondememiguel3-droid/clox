#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double __value_t__;

typedef struct {
  int capacity;
  int count;
  __value_t__ *values;
} __value_array_t__ ;

void init_value_array(__value_array_t__ *array);
void write_value_array(__value_array_t__ *array, __value_t__ value);
void free_value_array(__value_array_t__ *array);
void print_value(__value_t__ value);

#endif // clox_value_h
