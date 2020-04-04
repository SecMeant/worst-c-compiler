#include <cstdio>

#include "tokenizer.h"

using namespace wcc;

int
main(int argc, char** argv)
{
  if (argc < 2) {
    puts("BADARG");
    return -1;
  }

  Trie trie;

  for (auto i = 1; i < argc; ++i)
    trie.add_keyword(argv[i]);

  trie.fill_fallbacks();
  print_trie(*trie.root);
}
