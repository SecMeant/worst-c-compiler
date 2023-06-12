#pragma once

#include "ast.h"
#include <fmt/format.h>
#include <mipc/utils.h>

#include <variant>

#define COLOR_RED "\x1B[31m"
#define COLOR_GRN "\x1B[32m"
#define COLOR_YEL "\x1B[33m"
#define COLOR_BLU "\x1B[34m"
#define COLOR_MAG "\x1B[35m"
#define COLOR_CYN "\x1B[36m"
#define COLOR_WHT "\x1B[37m"
#define COLOR_RESET "\x1B[0m"

#define COLOR_ID COLOR_RED
#define COLOR_FIELD COLOR_CYN
#define COLOR_VALUE COLOR_YEL

using mipc::utils::underlay_cast;

template<>
struct fmt::formatter<wcc::AstVariable>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::AstVariable& astvar, FormatContext& ctx) const
  {
    // clang-format off
    return format_to(ctx.out(),
                     "<" COLOR_ID "ASTVariable" COLOR_RESET ": "
                     COLOR_FIELD "type" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                     COLOR_FIELD "name" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                     COLOR_FIELD "value" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ">",
                     wcc::LANG_TYPE_STR[underlay_cast(astvar.type)],
                     astvar.name,
                     astvar.value.u64_value);
    // clang-format on
  }
};

template<>
struct fmt::formatter<wcc::AstSymRef>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::AstSymRef& astsymref, FormatContext& ctx) const
  {
    // clang-format off
    return format_to(ctx.out(),
                     "<" COLOR_ID "AstSymRef" COLOR_RESET ": "
                     COLOR_FIELD "name" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ">",
                     astsymref.name);
    // clang-format on
  }
};

template<>
struct fmt::formatter<wcc::AstFunctionCall>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::AstFunctionCall& astcall, FormatContext& ctx) const
  {
    // clang-format off
    return format_to(ctx.out(),
                     "<" COLOR_ID "AstFunctionCall" COLOR_RESET ": "
                     COLOR_FIELD "name" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                     COLOR_FIELD "args" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", ",
                     astcall.name,
                     fmt::join(astcall.args, ", "));
    // clang-format on
  }
};

template<>
struct fmt::formatter<wcc::AstStmt>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::ASTNode::ValueStorage& value, FormatContext& ctx) const
  {
    auto& aststmt = std::get<wcc::AstStmt>(value);

    if (aststmt.type == wcc::StmtType::varref) {
      // clang-format off
      return format_to(ctx.out(),
                       "<" COLOR_ID "ASTStmt" COLOR_RESET ": "
                       COLOR_FIELD "type" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                       COLOR_FIELD "value" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ">",
                       wcc::STMT_TYPE_STR[underlay_cast(aststmt.type)], 
                       std::get<wcc::AstSymRef>(aststmt.value));
      // clang-format on
    } else if (aststmt.type == wcc::StmtType::call) {
      // clang-format off
      return format_to(ctx.out(),
                       "<" COLOR_ID "ASTStmt" COLOR_RESET ": "
                       COLOR_FIELD "type" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                       COLOR_FIELD "value" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ">",
                       wcc::STMT_TYPE_STR[underlay_cast(aststmt.type)], 
                       std::get<wcc::AstFunctionCall>(aststmt.value));
      // clang-format on
    } else if (aststmt.type == wcc::StmtType::ret) {
      // clang-format off
      return format_to(ctx.out(),
                       "<" COLOR_ID "ASTStmt" COLOR_RESET ": "
                       COLOR_FIELD "type" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET,
                       wcc::STMT_TYPE_STR[underlay_cast(aststmt.type)]);
      // clang-format on
    }

    return format_to(ctx.out(), "???");
  }
};

template<>
struct fmt::formatter<wcc::ASTNode::ValueStorage>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::ASTNode::ValueStorage& value, FormatContext& ctx) const
  {
    if (std::holds_alternative<wcc::AstVariable>(value)) {

      auto& astvar = std::get<wcc::AstVariable>(value);
      return format_to(ctx.out(), "{}", astvar);

    } else if (std::holds_alternative<wcc::AstFunction>(value)) {

      auto& astfunc = std::get<wcc::AstFunction>(value);

      // clang-format off
      return format_to(ctx.out(),
                       "<" COLOR_ID "ASTFunction" COLOR_RESET ": "
                       COLOR_FIELD "ret_type" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                       COLOR_FIELD "name" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                       COLOR_FIELD "args" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ">",
                       wcc::LANG_TYPE_STR[underlay_cast(astfunc.return_type)],
                       astfunc.name,
                       fmt::join(astfunc.args, ", "));
      // clang-format on

    } else if (std::holds_alternative<wcc::AstStruct>(value)) {

      auto& aststr = std::get<wcc::AstStruct>(value);

      // clang-format off
      return format_to(ctx.out(),
                       "<" COLOR_ID "ASTStruct" COLOR_RESET ": "
                       COLOR_FIELD "name" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                       COLOR_FIELD "fields" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ">",
                       aststr.name,
                       fmt::join(aststr.fields, ", "));
      // clang-format on

    } else if (std::holds_alternative<wcc::AstStmt>(value)) {

      auto& aststmt = std::get<wcc::AstStmt>(value);
      return format_to(ctx.out(), "{}", aststmt);

    }

    return format_to(ctx.out(), "???");
  }
};

template<>
struct fmt::formatter<wcc::ASTNode>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::ASTNode& node, FormatContext& ctx) const
  {
    // clang-format off
    return format_to(ctx.out(),
                     "<" COLOR_ID "ASTNode" COLOR_RESET ": "
                     COLOR_FIELD "id" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                     COLOR_FIELD "value" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ", "
                     COLOR_FIELD "node_count" COLOR_RESET "=" COLOR_VALUE "{}" COLOR_RESET ">",
                     wcc::ASTID_STR[underlay_cast(node.id)],
                     node.value,
                     node.nodes.size());
    // clang-format on
  }
};

template<>
struct fmt::formatter<wcc::AST>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::AST& ast, FormatContext& ctx) const
  {
    return format_to(ctx.out(), "{}", ast.root);
  }
};

struct AstFormatContext
{
  size_t level = 0;
};

inline void
print_ast_node(const wcc::ASTNode& node, AstFormatContext ctx)
{
  for (size_t i = 0; i < ctx.level; ++i)
    fmt::print("    ");

  fmt::print("{}\n", node);

  ++ctx.level;

  for (const auto& child : node.nodes)
    print_ast_node(*child, ctx);
}

inline void
print_ast(const wcc::AST& ast)
{
  AstFormatContext ctx;
  print_ast_node(ast.root, ctx);
}
