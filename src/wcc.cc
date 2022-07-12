#include <cstdio>
#include <map>

#include <fmt/format.h>
#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>

#include "mipc/file.h"
//#include "ahocorasick.h"
//#include "nfa.h"
#include "token_format.h"
#include "tokenizer.h"
#include "parser.h"

#include "ast_format.h"

using namespace wcc;
// using namespace wcc::regex;
using namespace mipc::utils;
using mipc::finbuf;

void
usage(int argc, char** argv)
{}

// int
// trie_main(int argc, char **argv)
//{
//  if (argc < 2) {
//    puts("BADARG");
//    return -1;
//  }
//
//  Trie trie;
//
//  for (auto i = 1; i < argc; ++i)
//    trie.add_keyword(argv[i]);
//
//  trie.fill_fallbacks();
//  print_trie(*trie.root);
//
//  return 0;
//}

AST ast;

int
tokenizer_main(int argc, char** argv)
{
  if (argc != 2) {
    usage(argc, argv);
    return 1;
  }

  finbuf    f(argv[1]);
  Tokenizer tokenizer(f.begin(), f.size());
  Parser    parser(tokenizer);

  //Tokenizer::breakpoints.emplace_back(2);

  ast = parser.buildAST();

  print_ast(ast);

  return 0;
}

int
main(int argc, char** argv)
{
  spdlog::cfg::load_env_levels();
  return tokenizer_main(argc, argv);
  // return trie_main(argc, argv);
  // nfa n;
  // n.append('a');
}
