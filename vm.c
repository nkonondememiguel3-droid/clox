#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include <stdio.h>

// global vm(virtual memory)
__vm_t__ vm;

void init_vm() {}

void free_vm() {}

static __interpret_result_t__ run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      __value_t__ constant = READ_CONSTANT();
      print_value(constant);
      printf("\n");
      break;
    }
    case OP_RETURN: {

      return INTERPRET_OK;
    }
    }
  }

#undef READ_CONSTANT
#undef READ_BYTE
}

__interpret_result_t__ interpret(__chunk_t__ *chunk) {

  vm.chunk = chunk;
  vm.ip = vm.chunk->code;

  return run();
}
