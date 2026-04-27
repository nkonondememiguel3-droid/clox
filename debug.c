#include "debug.h"
#include "chunk.h"
#include <stdint.h>
#include <stdio.h>

static int simple_instruction(const char *name, int offset) {

  printf("%s\n", name);
  return offset + 1;
}

static int constant_instruction(const char *name, __chunk_t__ *chunk,
                                int offset) {

  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");

  return offset + 2;
}

void disassemble_chunk(__chunk_t__ *chunk, const char *name) {

  printf("== %s ==\n", name);

  for (size_t offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}

int disassemble_instruction(__chunk_t__ *chunk, int offset) {

  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_RETURN:
    return simple_instruction("OP_RETURN", offset);
  case OP_CONSTANT:
    return constant_instruction("OP_CONSTANT", chunk, offset);
  default:
    printf("Unknow opcode %d\n", instruction);
    return offset + 1;
  }
}
