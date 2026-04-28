
#include "chunk.h"
#include "common.h"
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

  constant_idx = add_constant(&chunk, 10);
  write_chunk(&chunk, OP_CONSTANT, 124);
  write_chunk(&chunk, constant_idx, 124);
  write_chunk(&chunk, OP_NEGATE, 124);

  constant_idx = add_constant(&chunk, 3.4);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant_idx, 123);
  write_chunk(&chunk, OP_ADD, 123);
  constant_idx = add_constant(&chunk, 5.6);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant_idx, 123);
  write_chunk(&chunk, OP_DIVIDE, 123);

  write_chunk(&chunk, OP_RETURN, 125);

  interpret(&chunk);

  free_vm();
  free_chunk(&chunk);

  return 0;
}
