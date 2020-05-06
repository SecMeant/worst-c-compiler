#include "parser.h"

namespace wcc {

AST
Parser::buildAST()
{
  AST ast;
  Token token;

  do {

    token = tokenizer.get();

    switch (token.id) {


    }

  } while (token.id != TOKENID::END);

  return ast;
}

}
