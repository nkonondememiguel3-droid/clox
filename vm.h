#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

typedef struct {
  __chunk_t__ *chunk;

  uint_fast8_t *ip; // instruction pointer
} __vm_t__;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} __interpret_result_t__;

void init_vm();
__interpret_result_t__ interpret(__chunk_t__ *chunk);
void free_vm();

#endif // clox_vm_h
