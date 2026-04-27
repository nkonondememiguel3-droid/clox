#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassemble_chunk(__chunk_t__ *chunk, const char *name);
int disassemble_instruction(__chunk_t__ *chunk, int offset);

#endif // clox_debug_h
