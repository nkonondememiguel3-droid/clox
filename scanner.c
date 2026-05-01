#include "scanner.h"
#include "chunk.h"
#include "common.h"
#include <string.h>

typedef struct {
  const char *start;
  const char *current;
  int line;
} __scanner_t__;
__scanner_t__ __scanner__;

void init_scanner(const char *source) {
  __scanner__.start = source;
  __scanner__.current = source;
  __scanner__.line = 1;
}

//    static functins
static bool is_at_end(void) { return *__scanner__.current == '\0'; }

static __token_t__ make_token(__token_type_t__ type) {

  __token_t__ token;
  token.start = __scanner__.start;
  token.type = type;
  token.length = (int)(__scanner__.current - __scanner__.start);
  token.line = __scanner__.line;

  return token;
}

static __token_t__ error_token(const char *error_message) {

  __token_t__ token;
  token.type = TOKEN_ERROR;
  token.start = error_message;
  token.length = (int)strlen(error_message);
  token.line = __scanner__.line;

  return token;
}

static char advance(void) {
  __scanner__.current++;
  return __scanner__.current[-1];
}

static bool match(char expected) {
  if (is_at_end())
    return false;

  if (*__scanner__.current != expected)
    return false;

  __scanner__.current++;
  return true;
}

static char peek(void) { return *__scanner__.current; }

static char peek_next(void) {
  if (is_at_end())
    return '\0';
  return __scanner__.current[1];
}

static void skip_whitespace(void) {

  for (;;) {
    char c = peek();

    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;

    case '\n':
      __scanner__.line++;
      advance();
      break;

    case '/':
      if (peek_next() == '/') {
        while (peek() != '\n' && !is_at_end())
          advance();
      } else {
        return;
      }
      break;

    default:
      return;
    }
  }
}

static __token_t__ string(void) {

  while (peek() != '"' && !is_at_end()) {
    if (peek() == '\n')
      __scanner__.line++;
    advance();
  }

  if (is_at_end())
    return error_token("Unterminated string.");

  advance();
  return make_token(TOKEN_STRING);
}

static bool is_digit(char c) { return c >= '0' && c <= '9'; }

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static __token_t__ number(void) {

  while (is_digit(peek()))
    advance();

  if (peek() == '.' && is_digit(peek_next())) {
    advance();

    while (is_digit(peek()))
      advance();
  }

  return make_token(TOKEN_NUMBER);
}

static __token_type_t__ check_keyword(int start, int length, const char *rest,
                                      __token_type_t__ type) {

  if ((__scanner__.current - __scanner__.start) == (start + length) &&
      (memcmp(__scanner__.start + start, rest, (unsigned long)length) == 0)) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static __token_type_t__ identifier_type(void) {

  switch (__scanner__.start[0]) {
  case 'a':
    return check_keyword(1, 2, "nd", TOKEN_AND);
  case 'c':
    return check_keyword(1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return check_keyword(1, 3, "lse", TOKEN_ELSE);
  case 'f':
    if (__scanner__.current - __scanner__.start > 1) {
      switch (__scanner__.start[1]) {
      case 'a':
        return check_keyword(2, 3, "lse", TOKEN_FALSE);
      case 'o':
        return check_keyword(2, 1, "r", TOKEN_FOR);
      case 'u':
        return check_keyword(2, 1, "n", TOKEN_FUN);
      default:
        break;
      }
    }
    break;
  case 'i':
    return check_keyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return check_keyword(1, 2, "il", TOKEN_NIL);
  case 'o':
    return check_keyword(1, 1, "r", TOKEN_OR);
  case 'p':
    return check_keyword(1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return check_keyword(1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return check_keyword(1, 4, "uper", TOKEN_SUPER);
  case 't':
    if (__scanner__.current - __scanner__.start > 1) {
      switch (__scanner__.start[1]) {
      case 'h':
        return check_keyword(2, 2, "is", TOKEN_THIS);
      case 'r':
        return check_keyword(2, 2, "ue", TOKEN_TRUE);

      default:
        break;
      }
    }
    break;
  case 'v':
    return check_keyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return check_keyword(1, 4, "hile", TOKEN_WHILE);

  default:
    break;
  }

  return TOKEN_IDENTIFIER;
}

static __token_t__ identifier(void) {

  while (is_alpha(peek()) || is_digit(peek()))
    advance();

  return make_token(identifier_type());
}

// ------------------------
__token_t__ scan_token(void) {
  skip_whitespace();

  __scanner__.start = __scanner__.current;

  if (is_at_end())
    return make_token(TOKEN_EOF);

  char c = advance();

  if (is_alpha(c))
    return identifier();

  if (is_digit(c))
    return number();

  switch (c) {
  // Single character token.
  case '(':
    return make_token(TOKEN_LEFT_PAREN);
  case ')':
    return make_token(TOKEN_RIGHT_PAREN);
  case '{':
    return make_token(TOKEN_LEFT_BRACE);
  case '}':
    return make_token(TOKEN_RIGHT_BRACE);
  case ';':
    return make_token(TOKEN_SEMICOLON);
  case ',':
    return make_token(TOKEN_COMMA);
  case '.':
    return make_token(TOKEN_DOT);
  case '-':
    return make_token(TOKEN_MINUS);
  case '+':
    return make_token(TOKEN_PLUS);
  case '/':
    return make_token(TOKNE_SLASH);
  case '*':
    return make_token(TOKEN_STAR);

  // One or two tokens.
  case '!':
    return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<':
    return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '>':
    return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

  // literals
  case '"':
    return string();

  default:
    return make_token(TOKEN_EOF);
  }

  return error_token("Unexpected character.");
}
