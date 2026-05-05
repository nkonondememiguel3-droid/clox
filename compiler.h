#ifndef clox_compiler_h
#define clox_compiler_h

#include "chunk.h"
#include <stdbool.h>

bool compile(const char *source, __chunk_t__ *chunk);

#endif // clox_compiler_h
