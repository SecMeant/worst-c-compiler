#pragma once
#include "tokenizer.h"

/* GRAMMAR
 *
 * ATOMS:
 *   Functions
 *   Structures
 *   Variables
 *
 * FuncDef: TYPE IDENTIFIER PARENEXPR BLOCK
 *
 * StrDef: struct IDENTIFIER BLOCK
 *
 * VarDecl: TYPE IDENTIFIER SEMICOLON
 *
 * TYPE: IDENTIFIER {i8, i32, i64}
 */

namespace wcc {
struct Parser
{
  template<typename... Ts>
  Parser(Ts&&... args)
    : tokenizer(std::forward<Ts>(args)...)
  {}

  Tokenizer tokenizer;
};

}
