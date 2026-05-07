#include "compiler.h"
#include "chunk.h"
#include "common.h"
#include "scanner.h"
#include "value.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  __token_t__ current;
  __token_t__ previous;
  bool had_error;
  bool panic_mode;
} __parser_t__;

__parser_t__ parser;
__chunk_t__ *compiling_chunk;

static __chunk_t__ *current_chunk(void) { return compiling_chunk; }

static void emit_byte(uint8_t byte) {
  write_chunk(current_chunk(), byte, parser.previous.line);
}

static void emit_bytes(uint8_t byte_1, uint8_t byte_2) {
  emit_byte(byte_1);
  emit_byte(byte_2);
}

static void emit_return(void) { emit_byte(OP_RETURN); }

static void end_compiler(void) { emit_return(); }

static void error_at(__token_t__ *token, const char *message) {
  // if we already are in panic mode, we discard the current error
  if (parser.panic_mode)
    return;
  parser.panic_mode = true;

  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // nothing.
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.had_error = true;
}

static void error_at_current(const char *message) {
  error_at(&parser.current, message);
}

static void error(const char *message) { error_at(&parser.previous, message); }

static void advance(void) {

  parser.previous = parser.current;

  for (;;) {
    parser.current = scan_token();
    if (parser.current.type != TOKEN_ERROR)
      break;

    error_at_current(parser.current.start);
  }
}

static void consume(__token_type_t__ token_type, const char *message) {
  if (parser.current.type == token_type) {
    advance();
    return;
  }

  error_at_current(message);
}

static uint8_t make_constant(__value_t__ constant) {

  int constant_idx = add_constant(compiling_chunk, constant);
  if (constant_idx > UINT8_MAX) {
    error("Too many constants in a chunk.");
    return 0;
  }

  return (uint8_t)constant;
}

static void emit_constant(__value_t__ constant) {
  emit_bytes(OP_CONSTANT, make_constant(constant));
}

static void expression(void) {}

static void grouping(void) {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void number(void) {
  double value = strtod(parser.previous.start, NULL);
  emit_constant(value);
}

bool compile(const char *source, __chunk_t__ *chunk) {

  parser.had_error = false;
  parser.panic_mode = false;

  init_scanner(source);
  compiling_chunk = chunk;

  advance();

  expression();

  consume(TOKEN_EOF, "Expect end of expression");
  end_compiler();

  return !parser.had_error;
}
