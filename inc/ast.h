#pragma once

#include <map>
#include <memory>
#include <variant>
#include <vector>
#include <optional>

#include <mipc/utils.h>

namespace wcc {

using mipc::utils::underlay_cast;

enum class ASTID : size_t {
  empty,
  vardecl,
  funcdecl,
  strdecl,
  stmt,
};

constexpr const char *ASTID_STR[] = {
    [underlay_cast(ASTID::empty)] = "empty",
    [underlay_cast(ASTID::vardecl)] = "vardecl",
    [underlay_cast(ASTID::funcdecl)] = "funcdecl",
    [underlay_cast(ASTID::strdecl)] = "strdecl",
    [underlay_cast(ASTID::stmt)] = "stmt",
};

#define MakeLangType(type_name) lt_##type_name

enum class LangType : size_t {
  MakeLangType(void),
  MakeLangType(i8),
  MakeLangType(i16),
  MakeLangType(i32),
  MakeLangType(i64),
  MakeLangType(u8),
  MakeLangType(u16),
  MakeLangType(u32),
  MakeLangType(u64),
  MakeLangType(f32),
  MakeLangType(f64),
};

constexpr const char *LANG_TYPE_STR[] = {
    [underlay_cast(LangType::MakeLangType(void))] = "void",
    [underlay_cast(LangType::MakeLangType(i8))] = "i8",
    [underlay_cast(LangType::MakeLangType(i16))] = "i16",
    [underlay_cast(LangType::MakeLangType(i32))] = "i32",
    [underlay_cast(LangType::MakeLangType(i64))] = "i64",
    [underlay_cast(LangType::MakeLangType(u8))] = "u8",
    [underlay_cast(LangType::MakeLangType(u16))] = "u16",
    [underlay_cast(LangType::MakeLangType(u32))] = "u32",
    [underlay_cast(LangType::MakeLangType(u64))] = "u64",
    [underlay_cast(LangType::MakeLangType(f32))] = "f32",
    [underlay_cast(LangType::MakeLangType(f64))] = "f64",
};

inline const std::map<std::string, LangType> str_to_langtype_translation_map = {
    {"void", LangType::MakeLangType(void)},
    {"i8", LangType::MakeLangType(i8)},
    {"i16", LangType::MakeLangType(i16)},
    {"i32", LangType::MakeLangType(i32)},
    {"i64", LangType::MakeLangType(i64)},
    {"u8", LangType::MakeLangType(u8)},
    {"u16", LangType::MakeLangType(u16)},
    {"u32", LangType::MakeLangType(u32)},
    {"u64", LangType::MakeLangType(u64)},
    {"f32", LangType::MakeLangType(f32)},
    {"f64", LangType::MakeLangType(f64)},
};

inline std::optional<LangType> lookup_type(const std::string &type_name) {
  auto langtype_it = str_to_langtype_translation_map.find(type_name);

  if (langtype_it == str_to_langtype_translation_map.end())
    return std::nullopt;

  return langtype_it->second;
}

union VarValue {
  uint64_t u64_value;
};

using SymbolName = std::string;

struct AstSymRef {
  SymbolName name;
};

struct AstVariable {
  LangType type;
  SymbolName name;
  VarValue value;
};

struct AstFunction {
  using Args = std::vector<AstVariable>;

  LangType return_type;
  SymbolName name;
  Args args;
};

struct AstStruct {
  SymbolName name;

  using Fields = std::vector<AstVariable>;

  Fields fields;
};

struct AstFunctionCall {
  SymbolName name;

  using CallArgs = std::vector<AstSymRef>;

  CallArgs args;
};

enum class StmtType {
  varref,
  call,
};

constexpr const char *STMT_TYPE_STR[] = {
    [underlay_cast(StmtType::varref)] = "varref",
    [underlay_cast(StmtType::call)] = "call",
};

struct AstStmt {
  StmtType type;

  std::variant<AstSymRef, AstFunctionCall> value;
};


struct ASTNode {
  using WeakPointer = ASTNode *;
  using Reference = ASTNode &;
  using Pointer = std::unique_ptr<ASTNode>;
  using NodeArray = std::vector<Pointer>;
  using ValueStorage = std::variant<AstVariable, AstFunction, AstStruct, AstStmt>;

  ASTNode() : id(ASTID::empty), nodes() {}

  ASTNode(ASTID id) : id(id), nodes() {}

  ASTNode(const ASTNode &other) = default;
  ASTNode(ASTNode &&other) = default;

  ASTNode &operator=(const ASTNode &other) = default;
  ASTNode &operator=(ASTNode &&other) = default;

  Reference add(ASTID id) {
    return *nodes.emplace_back(std::make_unique<ASTNode>(id)).get();
  }

  ASTID id;
  NodeArray nodes;
  ValueStorage value;
};

struct AST {
  ASTNode root;
};

} // namespace wcc
