#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include <cassert>

#include <fmt/format.h>

namespace wcc {

using TokenDataType = char;

enum class TOKENID
{

};

struct Token
{
  using IdType        = TOKENID;
  using IdViewType    = TOKENID;
  using ValueType     = std::string;
  using ValueViewType = const std::string&;

  IdViewType get_id() { return id; }

  ValueViewType get_value() { return value; }

  TOKENID   id;
  ValueType value;
};

struct TrieNode
{
  using DataType       = TokenDataType;
  using NodeType       = TrieNode;
  using NodeRefType    = NodeType*;
  using NodeOwnRefType = std::unique_ptr<NodeType>;
  using IdentifierType = DataType;
  static_assert(std::is_convertible_v<decltype(nullptr), NodeRefType>);

  struct LookupInfoType
  {
    DataType       identifier;
    NodeOwnRefType item;

    LookupInfoType(DataType identifier, NodeOwnRefType item)
      : identifier(std::move(identifier))
      , item(std::move(item))
    {}

    LookupInfoType() = default;
  };

  template<typename T>
  using LookupContaierType = std::vector<T>;
  using LookupElementType  = LookupInfoType;
  using LookupType         = LookupContaierType<LookupElementType>;

private:
  TrieNode() = default;

public:
  TrieNode(NodeRefType parent, IdentifierType identifier)
    : parent(parent)
    , fallback(nullptr)
    , lookup()
    , identifier(identifier)
  {}

  ~TrieNode() = default;

  NodeRefType new_child(DataType identifier)
  {
    NodeOwnRefType node(new NodeType(this, identifier));
    lookup.emplace_back(identifier, std::move(node));
    return lookup.back().item.get();
  }

  //void add_child(DataType identifier, NodeType node)
  //{
  //  NodeOwnRefType node_ref(new NodeType); // TODO use make_unique
  //  add_child(std::move(identifier), std::move(node_ref));
  //}

  //void add_child(DataType identifier, NodeOwnRefType node)
  //{
  //  node->parent = this;
  //  lookup.emplace_back(std::move(identifier), std::move(node));
  //}

  NodeRefType find(DataType search)
  {
    for (auto& [identifier, node] : lookup) {
      if (search == identifier)
        return node.get();
    }

    return nullptr;
  }

  NodeRefType walk2root()
  {
    auto parent = this->parent;

    while (parent->parent != nullptr)
      parent = parent->parent;

    return parent;
  }

  void fill_fallback()
  {
    if (parent == nullptr) {
      return;
    }

    auto parents_fallback = parent->fallback;
    fmt::print("Parents fallback {}\n", (void*) parents_fallback);
    assert(parents_fallback != nullptr);
    auto node = parents_fallback->find(identifier);

    if (node == nullptr)
      node = this->walk2root();

    this->fallback = node;
  }

  bool is_final()
  {
    return this->lookup.size() == 0;
  }

  NodeRefType parent;
  NodeRefType fallback;
  LookupType  lookup;
  IdentifierType identifier;
};

struct Trie
{
  using NodeType             = TrieNode;
  using NodeRefType          = TrieNode*;
  using NodeOwnRefType       = std::unique_ptr<NodeType>;
  using KeywordRefType       = const std::string&;
  using KeywordCIteratorType = std::string::const_iterator;

  void create_branch(KeywordCIteratorType begin,
                     KeywordCIteratorType end,
                     NodeRefType          at)
  {
    while (begin != end) {
      auto identifier = *begin;
      at              = at->new_child(identifier);
      ++begin;
    }
  }

  void add_keyword(KeywordRefType kw)
  {
    NodeRefType current_node = root.get();

    auto       start = kw.cbegin();
    const auto end   = kw.cend();

    while (start != end) {
      auto identifier = *start;
      auto child      = current_node->find(identifier);

      ++start;

      if (child == nullptr) {
        current_node = current_node->new_child(identifier);
        create_branch(start, end, current_node);
        return;
      }

      current_node = child;
    }
  }

  static void fill_fallbacks_(NodeRefType root)
  {
    root->fill_fallback();
    for (auto& [_, child] : root->lookup) {
      fill_fallbacks_(child.get());
    }
  }

  void fill_fallbacks()
  {
    this->root->fallback = nullnode.get();
    fill_fallbacks_(this->root.get());
  }

  NodeOwnRefType nullnode = std::make_unique<NodeType>(nullptr, '\0');
  NodeOwnRefType root = std::make_unique<NodeType>(nullptr, '\0');
};

inline std::string format_trie_lookup(const TrieNode &node)
{
  std::string ret;

  for (auto &child : node.lookup) {
    ret += fmt::format("[{}:{}] ", child.identifier, (void*) child.item.get());
  }

  return ret;
}

inline void print_trie_node(const TrieNode &node)
{
  fmt::print(
      "@{}\n"
      "identifier:\t{}\n"
      "parent:\t{}\n"
      "fallback:\t{}\n"
      "lookup:\t{}\n"
      "\n"
      , (void*) &node, node.identifier, (void*) node.parent, (void*) node.fallback, format_trie_lookup(node)
  );
}

inline void print_trie(const TrieNode &root)
{
  print_trie_node(root);
  for (auto& [_, child] : root.lookup) {
    print_trie(*child);
  }
}

struct Tokenizer
{
  using DataType     = TokenDataType;
  using DataViewType = const TokenDataType*;
  using DataSizeType = size_t;
  using PositionType = DataSizeType;

  DataViewType data;
  DataSizeType data_size;
  PositionType pos;

  Tokenizer(DataViewType data, DataSizeType size)
    : data(data)
    , data_size(size)
    , pos(0)
  {}

  Token get();
  void  advance();
};

} // namespace wcc
