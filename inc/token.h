#pragma once

#include <mipc/utils.h>
#include <string>

#include <map>

namespace wcc {

using TokenDataType = char;

enum class TOKENID : size_t
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
  OP_LS,
  OP_LSE,
  OP_GR,
  OP_GRE,
  OP_DOT,
  BLOCK_BEGIN,
  BLOCK_END,
  SEMICOLON,
  COLON,
  COMMA,
  NAMESPACE,
  SINGLE_QUOTE,
  DOUBLE_QUOTE,
  IDENTIFIER,
  END,
};

using mipc::utils::underlay_cast;

constexpr const char* STDOP_FUNC_STR[] = {
  [underlay_cast(TOKENID::OP_NEG)] = "operatorNEG",
  [underlay_cast(TOKENID::OP_NEQ)] = "operatorNEQ",
  [underlay_cast(TOKENID::OP_MOD)] = "operatorMOD",
  [underlay_cast(TOKENID::OP_XOR)] = "operatorXOR",
  [underlay_cast(TOKENID::OP_LOGIC_AND)] = "operatorLOGIC_AND",
  [underlay_cast(TOKENID::OP_ANDEQ)] = "operatorANDEQ",
  [underlay_cast(TOKENID::OP_AND)] = "operatorAND",
  [underlay_cast(TOKENID::OP_LOGIC_OR)] = "operatorLOGIC_OR",
  [underlay_cast(TOKENID::OP_OREQ)] = "operatorOREQ",
  [underlay_cast(TOKENID::OP_OR)] = "operatorOR",
  [underlay_cast(TOKENID::OP_MUL)] = "operatorMUL",
  [underlay_cast(TOKENID::OP_MULEQ)] = "operatorMULEQ",
  [underlay_cast(TOKENID::OP_DIVEQ)] = "operatorDIVEQ",
  [underlay_cast(TOKENID::OP_DIV)] = "operatorDIV",
  [underlay_cast(TOKENID::OP_MINUS)] = "operatorMINUS",
  [underlay_cast(TOKENID::OP_ACCESS)] = "operatorACCESS",
  [underlay_cast(TOKENID::OP_PLUS)] = "operatorPLUS",
  [underlay_cast(TOKENID::OP_EQ)] = "operatorEQ",
  [underlay_cast(TOKENID::OP_LS)] = "operatorLS",
  [underlay_cast(TOKENID::OP_LSE)] = "operatorLSE",
  [underlay_cast(TOKENID::OP_GR)] = "operatorGR",
  [underlay_cast(TOKENID::OP_GRE)] = "operatorGRE",
  [underlay_cast(TOKENID::OP_DOT)] = "operatorDOT",
};

constexpr const char* TOKENID_STR[] = {
  [underlay_cast(TOKENID::PAREN_OPEN)] = "PAREN_OPEN",
  [underlay_cast(TOKENID::PAREN_CLOSE)] = "PAREN_CLOSE",
  [underlay_cast(TOKENID::BRACKET_OPEN)] = "BRACKET_OPEN",
  [underlay_cast(TOKENID::BRACKET_CLOSE)] = "BRACKET_CLOSE",
  [underlay_cast(TOKENID::OP_NEG)] = "OP_NEG",
  [underlay_cast(TOKENID::OP_NEQ)] = "OP_NEQ",
  [underlay_cast(TOKENID::OP_MOD)] = "OP_MOD",
  [underlay_cast(TOKENID::OP_XOR)] = "OP_XOR",
  [underlay_cast(TOKENID::OP_LOGIC_AND)] = "OP_LOGIC_AND",
  [underlay_cast(TOKENID::OP_ANDEQ)] = "OP_ANDEQ",
  [underlay_cast(TOKENID::OP_AND)] = "OP_AND",
  [underlay_cast(TOKENID::OP_LOGIC_OR)] = "OP_LOGIC_OR",
  [underlay_cast(TOKENID::OP_OREQ)] = "OP_OREQ",
  [underlay_cast(TOKENID::OP_OR)] = "OP_OR",
  [underlay_cast(TOKENID::OP_MUL)] = "OP_MUL",
  [underlay_cast(TOKENID::OP_MULEQ)] = "OP_MULEQ",
  [underlay_cast(TOKENID::OP_DIVEQ)] = "OP_DIVEQ",
  [underlay_cast(TOKENID::OP_DIV)] = "OP_DIV",
  [underlay_cast(TOKENID::OP_MINUS)] = "OP_MINUS",
  [underlay_cast(TOKENID::OP_ACCESS)] = "OP_ACCESS",
  [underlay_cast(TOKENID::OP_PLUS)] = "OP_PLUS",
  [underlay_cast(TOKENID::OP_EQ)] = "OP_EQ",
  [underlay_cast(TOKENID::BLOCK_BEGIN)] = "BLOCK_BEGIN",
  [underlay_cast(TOKENID::BLOCK_END)] = "BLOCK_END",
  [underlay_cast(TOKENID::SEMICOLON)] = "SEMICOLON",
  [underlay_cast(TOKENID::COLON)] = "COLON",
  [underlay_cast(TOKENID::COMMA)] = "COMMA",
  [underlay_cast(TOKENID::NAMESPACE)] = "NAMESPACE",
  [underlay_cast(TOKENID::SINGLE_QUOTE)] = "SINGLE_QUOTE",
  [underlay_cast(TOKENID::DOUBLE_QUOTE)] = "DOUBLE_QUOTE",
  [underlay_cast(TOKENID::OP_LS)] = "OP_LS",
  [underlay_cast(TOKENID::OP_LSE)] = "OP_LSE",
  [underlay_cast(TOKENID::OP_GR)] = "OP_GR",
  [underlay_cast(TOKENID::OP_GRE)] = "OP_GRE",
  [underlay_cast(TOKENID::OP_DOT)] = "OP_DOT",
  [underlay_cast(TOKENID::IDENTIFIER)] = "IDENTIFIER",
  [underlay_cast(TOKENID::END)] = "END",
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
