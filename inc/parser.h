#pragma once

#include <set>

#include "tokenizer.h"
#include "ast.h"

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
 *
 * BLOCK: { STMTS }
 *
 * STMTS: STMT STMTS | STMT
 *
 * EXPR: E OP EXPR | E
 *
 */

namespace wcc {

inline const std::set<Token::ValueType> KEYWORDS = {
        "struct",

        "void",
        "i8",
        "i16",
        "i32",
        "i64",
        "u8",
        "u16",
        "u32",
        "u64",
};

struct Parser
{
  template<typename... Ts>
  Parser(Ts&&... args)
    : tokenizer(std::forward<Ts>(args)...)
  {}

  Parser(Tokenizer tokenizer)
    : tokenizer(std::move(tokenizer))
  {}

  AST buildAST();

  Tokenizer tokenizer;
};

}
