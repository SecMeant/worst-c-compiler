#include <cctype>

#include "token_format.h"
#include "tokenizer.h"
#include "util.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace wcc {

static void
breakpoint()
{}

constexpr static bool
is_identifier(char c)
{
  switch (c) {
    case '0' ... '9':
    case '_' ... '`':
      // If we include '`' then the range is continuous, thus faster parsing
    case 'a' ... 'z':
    case 'A' ... 'Z':
      return true;
    default:
      return false;
  }
}

void
Tokenizer::consume_ws()
{
  char c;

  while (1) {
    if (current == end)
      return;

    c = *current;

    if (!std::isspace(c))
      return;

    if (c == '\n') {
      ++line;
      pos = 0;
    }

    if (std::find(std::begin(breakpoints), std::end(breakpoints), line) !=
        std::end(breakpoints))
      breakpoint();

    ++current;
    ++pos;
  }
}

Token
Tokenizer::get()
{
  // ASSUME that current points at character not yet parsed
  Token ret;
  char  c;

  OnBlockExit([&ret] { spdlog::debug("{}", ret); });

  ret.id   = TOKENID::END;
  ret.line = line;
  ret.pos  = pos;

  if (current == end)
    return ret;

  auto next = [](DataViewType current) { return *(current + 1); };

match_token:

  c = *current;
  ++current;
  switch (c) {
    case '(':
      ret.id = TOKENID::PAREN_OPEN;
      break;
    case ')':
      ret.id = TOKENID::PAREN_CLOSE;
      break;
    case '[':
      ret.id = TOKENID::BRACKET_OPEN;
      break;
    case ']':
      ret.id = TOKENID::BRACKET_CLOSE;
      break;
    case '!':
      ret.id = TOKENID::OP_NEG;

      if (next(current) == '=') {
        ++current;
        ret.id = TOKENID::OP_NEQ;
      }

      break;
    case '%':
      ret.id = TOKENID::OP_MOD;
      break;
    case '^':
      ret.id = TOKENID::OP_XOR;
      break;
    case '&':
      switch (next(current)) {
        case '&':
          ret.id = TOKENID::OP_LOGIC_AND;
          ++current;
          break;
        case '=':
          ret.id = TOKENID::OP_ANDEQ;
          ++current;
          break;
        default:
          ret.id = TOKENID::OP_AND;
      }

      break;
    case '|':
      switch (next(current)) {
        case '|':
          ret.id = TOKENID::OP_LOGIC_OR;
          ++current;
          break;
        case '=':
          ret.id = TOKENID::OP_OREQ;
          ++current;
          break;
        default:
          ret.id = TOKENID::OP_OR;
      }

    case '*':
      ret.id = TOKENID::OP_MUL;

      if (next(current) == '=') {
        ret.id = TOKENID::OP_MULEQ;
        ++current;
      }

      break;
    case '/':
      switch (next(current)) {
        case '=':
          ret.id = TOKENID::OP_DIVEQ;
          ++current;
          break;
        case '/':
          // Endline might be escaped
          while (next(current) != '\n' && *current != '\\')
            ++current;
          consume_ws();
          goto match_token;
        default:
          ret.id = TOKENID::OP_DIV;
      }

      break;
    case '-':
      ret.id = TOKENID::OP_MINUS;

      if (next(current) == '>') {
        ret.id = TOKENID::OP_ACCESS;
        ++current;
      }

      break;
    case '+':
      ret.id = TOKENID::OP_PLUS;
      break;
    case '=':
      ret.id = TOKENID::OP_EQ;
      break;
    case '{':
      ret.id = TOKENID::BLOCK_BEGIN;
      break;
    case '}':
      ret.id = TOKENID::BLOCK_END;
      break;
    case ';':
      ret.id = TOKENID::SEMICOLON;
      break;
    case ':':
      ret.id = TOKENID::COLON;

      if (next(current) == ':') {
        ret.id = TOKENID::NAMESPACE;
        ++current;
      }

      break;
    case ',':
      ret.id = TOKENID::COMMA;
      break;
    case '\'':
      ret.id = TOKENID::SINGLE_QUOTE;
      break;
    case '\"':
      ret.id = TOKENID::DOUBLE_QUOTE;
      break;
    case '<':
      ret.id = TOKENID::OP_LS;

      if (next(current) == '=') {
        ret.id = TOKENID::OP_LSE;
        ++current;
      }

      break;
    case '>':
      ret.id = TOKENID::OP_GR;

      if (next(current) == '=') {
        ret.id = TOKENID::OP_GRE;
        ++current;
      }

      break;
    case '.':
      ret.id = TOKENID::OP_DOT;
      break;
    default:
      ret.id    = TOKENID::IDENTIFIER;
      ret.value = "";

      for (;;) {
        ret.value += c;

        c = *current;

        if (!is_identifier(c) || current == end)
          break;

        ++current;
      }
  }

  consume_ws();
  return ret;
}

}
