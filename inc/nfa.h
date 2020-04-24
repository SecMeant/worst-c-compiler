#pragma once

#include <array>
#include <cstdint>

#include "util.h"

static constexpr std::size_t NFA_MAX_RANK = 16;

namespace wcc::regex {

using nfa_key_t = uint64_t;

constexpr nfa_key_t epsilon = 0;

struct nfa_node
{
  using KeyType = nfa_key_t;

  struct NodeArrayElement
  {
    KeyType   key   = epsilon;
    nfa_node* value = nullptr;
  };

  using NodeArray = std::array<NodeArrayElement, NFA_MAX_RANK>;

  nfa_node(KeyType s)
    : key(s)
  {}

  void add(NodeArrayElement e);
  nfa_node* add(KeyType s);

  ~nfa_node()
  {
    for (auto e : edges)
      delete e.value;
  }

  NodeArray   edges;
  std::size_t rank = 0;
  KeyType     key;
  bool        accepting = false;
};

template<typename T, size_t N>
struct stack
{
  constexpr static size_t max_size = N;

  template<typename U>
  void push(U&& e)
  {
    if (top_idx == max_size)
      panic("Stack overflow");

    values[top_idx] = std::forward<U>(e);
    ++top_idx;
  }

  T pop()
  {
    if (top_idx == 0)
      panic("Stack underflow");

    --top_idx;
    return std::move(values[top_idx]);
  }

  const T& top() { return values[top_idx - 1]; }

  T      values[max_size];
  size_t top_idx;
};

struct nfa_context
{
  nfa_node *begin, *end;

  static nfa_context make_root(nfa_node* n) { return { n, n }; }

  static nfa_context make_root()
  {
    auto root = new nfa_node(epsilon);
    return { root, root };
  }
};

struct nfa
{
  constexpr static size_t max_node_stack_size = 16;

  using NodeStack = stack<nfa_node*, max_node_stack_size>;

  nfa()
    : context(nfa_context::make_root())
  {}

  ~nfa() { delete context.begin; }

  void walk();
  void append(nfa_node::KeyType k);

  nfa_context context;
  NodeStack   stacks[2];
  unsigned    stack_idx = 0;
};

} // namespace wcc
