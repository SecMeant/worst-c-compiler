#include <cctype>

#include "tokenizer.h"

namespace wcc {

constexpr static bool is_identifier(char c)
{
  switch(c) {
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

Token
Tokenizer::get()
{
  // ASSUME that current points at character not yet parsed
  
  Token ret;
  char c;

skip_whitespace:

  do {
    if (current == end) {
      ret.id = TOKENID::END;
      return ret;
    }

    c = *current;
    ++current;
    ++pos;

    if (c == '\n') {
      ++line;
      pos = 0;
    }
  } while (std::isspace(c));

  ret.line = line;
  ret.pos  = pos;

  auto next = [](DataViewType current) { return *(current + 1); };

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
          // TODO(holz) optimize by combining this as read and check on uint16
          while (next(current) != '\n' && *current != '\\')
            ++current;
          goto skip_whitespace;
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
      ret.value += c;

      for(;;) {
        c = *current;

        if (!is_identifier(c) || current == end)
          break;

        ret.value += c;
        ++current;
      }
  }

  return ret;
}

}
