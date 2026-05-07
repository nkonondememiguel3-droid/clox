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

// all the clox precedence operator in order;.
typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARAISON, // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} __precedence_type__;

typedef void (*__parser_fn__)(void);

typedef struct {
  __parser_fn__ prefix;
  __parser_fn__ infix;
  __precedence_type__ precedence;
} __parse_rule__;

static void expression();
static __parse_rule__ *get_rule(__token_type_t__ type);
static void parse_precedence(__precedence_type__ precedence);

static __chunk_t__ *current_chunk(void) { return compiling_chunk; }

static void emit_byte(uint8_t byte) {
  write_chunk(current_chunk(), byte, parser.previous.line);
}

static void emit_bytes(uint8_t byte_1, uint8_t byte_2) {
  emit_byte(byte_1);
  emit_byte(byte_2);
}

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

static void parse_precedence(__precedence_type__ precedence) {}

static void binary(void) {

  __token_type_t__ operator_type = parser.previous.type;
  __parse_rule__ *rule = get_rule(operator_type);

  parse_precedence((__precedence_type__)(rule->precedence + 1));

  switch (operator_type) {
  case TOKEN_PLUS:
    emit_byte(OP_ADD);
    break;
  case TOKEN_MINUS:
    emit_byte(OP_SUBTRACT);
    break;
  case TOKEN_STAR:
    emit_byte(OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    emit_byte(OP_DIVIDE);
    break;

  default:
    return;
  }
}

static void error(const char *message) { error_at(&parser.previous, message); }

static void error_at_current(const char *message) {
  error_at(&parser.current, message);
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

static void expression(void) { parse_precedence(PREC_ASSIGNMENT); }

static void unary(void) {
  __token_type_t__ operator_type = parser.previous.type;

  // compile the operand.
  parse_precedence(PREC_UNARY);

  // emit the operator instruction.
  switch (operator_type) {
  case TOKEN_MINUS:
    emit_byte(OP_NEGATE);
    break;

  default:
    return;
  }
}

static void number(void) {
  double value = strtod(parser.previous.start, NULL);
  emit_constant(value);
}

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

static void grouping(void) {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

__parse_rule__ rules[] = {
    [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
    [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
    [TOKEN_MINUS] = {unary, binary, PREC_TERM},
    [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
    [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
    [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
    [TOKEN_BANG] = {NULL, NULL, PREC_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
    [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
    [TOKEN_AND] = {NULL, NULL, PREC_NONE},
    [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FALSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
    [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
    [TOKEN_IF] = {NULL, NULL, PREC_NONE},
    [TOKEN_NIL] = {NULL, NULL, PREC_NONE},
    [TOKEN_OR] = {NULL, NULL, PREC_NONE},
    [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
    [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
    [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
    [TOKEN_TRUE] = {NULL, NULL, PREC_NONE},
    [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
    [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};

static __parse_rule__ *get_rule(__token_type_t__ type) {
  return &rules[type];
}

static void emit_return(void) { emit_byte(OP_RETURN); }

static void end_compiler(void) { emit_return(); }

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
