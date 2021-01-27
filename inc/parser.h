#pragma once

#include <set>

#include "tokenizer.h"
#include "ast.h"

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
