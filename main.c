
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, char **argv) {

  init_vm();
  __chunk_t__ chunk;

  init_chunk(&chunk);

  int constant_idx = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant_idx, 123);

  constant_idx = add_constant(&chunk, 3.14);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant_idx, 123);

  write_chunk(&chunk, OP_RETURN, 124);

  interpret(&chunk);
  /* disassemble_chunk(&chunk, "test chunk"); */

  free_vm();
  free_chunk(&chunk);

  return 0;
}
