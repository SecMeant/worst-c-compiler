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

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

using llvm_context_t = llvm::LLVMContext;
using llvm_module_t  = llvm::Module;

using namespace wcc;
// using namespace wcc::regex;
using namespace mipc::utils;
using mipc::finbuf;

static void
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

static int
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

static std::unique_ptr<llvm_context_t> llvm_context;
static std::unique_ptr<llvm_module_t> llvm_module;

static void init_llvm()
{
  llvm_context = std::make_unique<llvm_context_t>();
  llvm_module = std::make_unique<llvm_module_t>("wcc", *llvm_context);
}

static int
gencode()
{
  init_llvm();

  return 0;
}

int
main(int argc, char** argv)
{
  spdlog::cfg::load_env_levels();

  const int tokenizer_ret = tokenizer_main(argc, argv);
  if (tokenizer_ret) {
    fprintf(stderr, "Tokenizer result non-zero\n");
    return tokenizer_ret;
  }

  const int gencode_ret = gencode();
  if (gencode_ret) {
    fprintf(stderr, "Code generation result non-zero\n");
    return gencode_ret;
  }

  // return trie_main(argc, argv);
  // nfa n;
  // n.append('a');
}
