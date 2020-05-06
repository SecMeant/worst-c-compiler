#pragma once
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
struct Parser
{
  template<typename... Ts>
  Parser(Ts&&... args)
    : tokenizer(std::forward<Ts>(args)...)
  {}

  AST buildAST();

  Tokenizer tokenizer;
};

}
