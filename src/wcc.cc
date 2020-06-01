#include <cstdio>
#include <map>

#include <fmt/format.h>

#include "mipc/file.h"
//#include "ahocorasick.h"
//#include "nfa.h"
#include "token_format.h"
#include "tokenizer.h"

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

int
tokenizer_main(int argc, char** argv)
{
  if (argc != 2) {
    usage(argc, argv);
    return 1;
  }

  finbuf    f(argv[1]);
  Tokenizer tokenizer(f.begin(), f.size());
  Token     token;

  do {
    token = tokenizer.get();
    fmt::print("{}\n", token);
  } while (token.id != TOKENID::END);

  return 0;
}

int
main(int argc, char** argv)
{
  return tokenizer_main(argc, argv);
  // return trie_main(argc, argv);
  // nfa n;
  // n.append('a');
}
