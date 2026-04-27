#include "chunk.h"
#include "memory.h"
#include "value.h"
#include <stdint.h>

void init_chunk(__chunk_t__ *chunk) {

  chunk->code = NULL;
  chunk->count = 0;
  chunk->capacity = 0;

  init_value_array(&chunk->constants);

  chunk->lines = NULL;
}

void write_chunk(__chunk_t__ *chunk, uint8_t byte, int line) {

  if (chunk->capacity < chunk->count + 1) {
    int old_capacity = chunk->capacity;

    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    chunk->lines =
        GROW_ARRAY(int, chunk->lines, old_capacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

void free_chunk(__chunk_t__ *chunk) {

  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

int add_constant(__chunk_t__ *chunk, __value_t__ value) {

  write_value_array(&chunk->constants, value);
  // return the index where the constant was store so that we can locate it
  // later.
  return chunk->constants.count - 1;
}
