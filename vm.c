#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include <stdio.h>

// global vm(virtual memory)
__vm_t__ vm;

static void reset_stack(void) { vm.stack_top = vm.stack; }

void init_vm(void) { reset_stack(); }

void free_vm(void) {}

void push(__value_t__ value) {

  *vm.stack_top = value;
  vm.stack_top++;
}

__value_t__ pop(void) {

  vm.stack_top--;
  return *vm.stack_top;
}

static __interpret_result_t__ run(void) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = pop();                                                          \
    double a = pop();                                                          \
    push(a op b);                                                              \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (__value_t__ *slot = vm.stack; slot < vm.stack_top; slot++) {
      printf("[ ");
      print_value(*slot);
      printf(" ]");
    }
    printf("\n");

    disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {

      __value_t__ constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_NEGATE: {
      push(-pop());
      break;
    }
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_RETURN: {

      print_value(pop());
      printf("\n");
      return INTERPRET_OK;
    }
    default:
      break;
    }
  }

#undef READ_CONSTANT
#undef READ_BYTE
#undef BINARY_OP
}

__interpret_result_t__ interpret(const char *source) {

  __chunk_t__ chunk;
  init_chunk(&chunk);

  if ( !compile(source, &chunk) ) {
    free_chunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  __interpret_result_t__ result = run();

  free_chunk(&chunk);
  return result;
}
