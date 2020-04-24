#include "nfa.h"
#include "util.h"

namespace wcc::regex {
void
nfa_node::add(NodeArrayElement e)
{
  if (unlikely(rank == NFA_MAX_RANK))
    panic("nfa rank excedded");

  edges[rank] = e;
  ++rank;
}

nfa_node*
nfa_node::add(KeyType s)
{
  if (unlikely(rank == NFA_MAX_RANK))
    panic("nfa rank excedded");

  nfa_node* n = new nfa_node(s);
  add({ s, n });
  return n;
}

void
nfa::append(nfa_node::KeyType k)
{
  auto n = context.end->add(k);
  context.end = n;
}
}
