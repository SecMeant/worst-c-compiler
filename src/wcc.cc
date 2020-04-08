#include <cstdio>
#include <map>

#include <fmt/format.h>

#include "mipc/file.h"
#include "tokenizer.h"

using namespace wcc;
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

constexpr static const char* TOKENID_STR[] = {
  "PAREN_OPEN", "PAREN_CLOSE", "BRACKET_OPEN", "BRACKET_CLOSE", "OP_NEG",
  "OP_NEQ",     "OP_MOD",      "OP_XOR",       "OP_LOGIC_AND",  "OP_ANDEQ",
  "OP_AND",     "OP_LOGIC_OR", "OP_OREQ",      "OP_OR",         "OP_MUL",
  "OP_MULEQ",   "OP_DIVEQ",    "OP_DIV",       "OP_MINUS",      "OP_ACCESS",
  "OP_PLUS",    "OP_EQ",       "BLOCK_BEGIN",  "BLOCK_END",     "SEMICOLON",
  "COLON",      "COMMA",       "NAMESPACE",    "SINGLE_QUOTE",  "DOUBLE_QUOTE",
  "OP_LS",      "OP_LSE",      "OP_GR",        "OP_GRE",        "OP_DOT",
  "IDENTIFIER", "END",
};

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
main(int argc, char** argv)
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
}
