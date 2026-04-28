#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
  __chunk_t__ *chunk;

  uint_fast8_t *ip; // instruction pointer

  __value_t__ stack[STACK_MAX];
  __value_t__ *stack_top;
} __vm_t__;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} __interpret_result_t__;

void init_vm();
__interpret_result_t__ interpret(const char *source);
void push(__value_t__ value);
__value_t__ pop();
void free_vm();

#endif // clox_vm_h
