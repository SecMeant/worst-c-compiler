#include <cstdio>
#include <map>

#include <fmt/format.h>

#include "mipc/file.h"
#include "nfa.h"
#include "tokenizer.h"

using namespace wcc;
using namespace wcc::regex;
using namespace mipc::utils;
using mipc::finbuf;

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
//}

void
print(Token t)
{
  fmt::print(t.id == TOKENID::IDENTIFIER ? "id: {} \"{}\"\n" : "id: {}\n",
             TOKENID_STR[mipc::utils::underlay_cast(t.id)],
             t.value);
}

void
usage(int argc, char** argv)
{}

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
    print(token);
  } while (token.id != TOKENID::END);

  return 0;
}

int
main(int argc, char** argv)
{
  return tokenizer_main(argc, argv);
  nfa n;
  n.append('a');
}
