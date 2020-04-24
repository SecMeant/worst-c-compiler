#pragma once

#include <string>

namespace wcc {

using TokenDataType = char;

enum class TOKENID
{
  PAREN_OPEN = 0,
  PAREN_CLOSE,
  BRACKET_OPEN,
  BRACKET_CLOSE,
  OP_NEG,
  OP_NEQ,
  OP_MOD,
  OP_XOR,
  OP_LOGIC_AND,
  OP_ANDEQ,
  OP_AND,
  OP_LOGIC_OR,
  OP_OREQ,
  OP_OR,
  OP_MUL,
  OP_MULEQ,
  OP_DIVEQ,
  OP_DIV,
  OP_MINUS,
  OP_ACCESS,
  OP_PLUS,
  OP_EQ,
  BLOCK_BEGIN,
  BLOCK_END,
  SEMICOLON,
  COLON,
  COMMA,
  NAMESPACE,
  SINGLE_QUOTE,
  DOUBLE_QUOTE,
  OP_LS,
  OP_LSE,
  OP_GR,
  OP_GRE,
  OP_DOT,
  IDENTIFIER,
  END,
};

constexpr const char* TOKENID_STR[] = {
  "PAREN_OPEN", "PAREN_CLOSE", "BRACKET_OPEN", "BRACKET_CLOSE", "OP_NEG",
  "OP_NEQ",     "OP_MOD",      "OP_XOR",       "OP_LOGIC_AND",  "OP_ANDEQ",
  "OP_AND",     "OP_LOGIC_OR", "OP_OREQ",      "OP_OR",         "OP_MUL",
  "OP_MULEQ",   "OP_DIVEQ",    "OP_DIV",       "OP_MINUS",      "OP_ACCESS",
  "OP_PLUS",    "OP_EQ",       "BLOCK_BEGIN",  "BLOCK_END",     "SEMICOLON",
  "COLON",      "COMMA",       "NAMESPACE",    "SINGLE_QUOTE",  "DOUBLE_QUOTE",
  "OP_LS",      "OP_LSE",      "OP_GR",        "OP_GRE",        "OP_DOT",
  "IDENTIFIER", "END",
};

struct Token
{
  using IdType       = TOKENID;
  using ValueType    = std::string;
  using PositionType = size_t;

  TOKENID      id;
  PositionType line, pos;
  ValueType    value;
};

};
