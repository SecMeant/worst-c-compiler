#pragma once

#include <memory>
#include <vector>

namespace wcc {

enum class ASTID
{
  empty,

};

struct ASTNode
{
  using WeakPointer = ASTNode*;
  using Reference   = ASTNode&;
  using Pointer     = std::unique_ptr<ASTNode>;
  using NodeArray   = std::vector<Pointer>;

  ASTNode()
    : id(ASTID::empty)
    , nodes()
  {}

  ASTNode(ASTID id)
    : id(id)
    , nodes()
  {}

  Reference add(ASTID id) { return *nodes.emplace_back(std::make_unique<ASTNode>(id)).get(); }

  ASTID     id;
  NodeArray nodes;
};

struct AST
{
  ASTNode root;
};

}
