#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// the operation code controls what kind of instruction we're dealing with.
// (add, substract, look up variable, etc)
typedef enum {
  OP_RETURN,
  OP_NEGATE,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_CONSTANT,
} __op_code_t__;

// bytecode is a serie of instructions.
typedef struct {
  int count;
  int capacity;

  uint8_t *code;

  // constant pool
  __value_array_t__ constants;

  // line numbers
  int *lines;
} __chunk_t__;

void init_chunk(__chunk_t__ *chunk);
void free_chunk(__chunk_t__ *chunk);
void write_chunk(__chunk_t__ *chunk, uint8_t byte, int line);
int add_constant(__chunk_t__ *chunk, __value_t__ value);

#endif // clox_chunk_h
