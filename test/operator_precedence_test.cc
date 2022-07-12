#include <cstdlib>

#include <fmt/format.h>

#include "ast.h"
#include "ast_format.h"
#include "parser.h"
#include "tokenizer.h"
#include "util.h"

#include "test.h"

#if defined(__unix__)
#define PATH_SEPARATOR "/"
#else
#error "Path separator not defined for this target"
#endif

using namespace wcc;

const char testsrc[] = "void func() {\n"
                       "u64 a;\n"
                       "u64 b;\n"
                       "u64 c;\n"
                       "a = 10 + 20 * 30 + 5;\n"
                       "b = 12 * 13 + 4;\n"
                       "}\n";

struct test_ctx
{
  struct
  {
    size_t muls_checked = 0;
    size_t adds_checked = 0;
    bool   arg_10       = false;
    bool   arg_5        = false;
  } first_assignment;

  struct
  {
    size_t muls_checked = 0;
    size_t adds_checked = 0;
  } second_assignment;
};

static bool
check_first_assign_(struct test_ctx& ctx, const AstStmt& stmt)
{
  const auto& call = std::get<AstFunctionCall>(stmt.value);

  if (call.args[0].type == StmtType::call)
    TEST_ASSERT(check_first_assign_(ctx, call.args[0]));

  if (call.args[1].type == StmtType::call)
    TEST_ASSERT(check_first_assign_(ctx, call.args[1]));

  if (call.name == STDOP_FUNC_STR[underlay_cast(TOKENID::OP_MUL)]) {
    TEST_ASSERT(ctx.first_assignment.muls_checked < 1);
    TEST_ASSERT(call.args.size() == 2);

    bool arg_20 = false, arg_30 = false;

    for (const auto& arg : call.args) {
      TEST_ASSERT(arg.type == StmtType::varref);

      arg_20 |= std::get<AstSymRef>(arg.value).name == "20";
      arg_30 |= std::get<AstSymRef>(arg.value).name == "30";
    }

    TEST_ASSERT(arg_20);
    TEST_ASSERT(arg_30);

    ++ctx.first_assignment.muls_checked;
  }

  if (call.name == STDOP_FUNC_STR[underlay_cast(TOKENID::OP_PLUS)]) {
    TEST_ASSERT(call.args.size() == 2);

    for (const auto& arg : call.args) {
      if (arg.type != StmtType::varref)
        continue;

      // Make sure we have seen arg 10 and arg 5 only once and never at the
      // same time as an argument to the same operator plus.
      const auto arg_10_local =
        std::get<AstSymRef>(call.args[0].value).name == "10";
      const auto arg_5_local =
        std::get<AstSymRef>(call.args[0].value).name == "5";

      ctx.first_assignment.arg_10 += arg_10_local;
      ctx.first_assignment.arg_5 += arg_5_local;

      TEST_ASSERT((arg_10_local ^ arg_5_local));
    }

    ++ctx.first_assignment.adds_checked;
  }

  return true;
}

static bool
check_first_assign(struct test_ctx& ctx, const AstStmt& stmt)
{
  check_first_assign_(ctx, stmt);

  TEST_ASSERT(ctx.first_assignment.adds_checked == 2);
  TEST_ASSERT(ctx.first_assignment.muls_checked == 1);
  TEST_ASSERT(ctx.first_assignment.arg_10 == 1);
  TEST_ASSERT(ctx.first_assignment.arg_5 == 1);

  return true;
}

static bool
check_second_assign_(struct test_ctx& ctx, const AstStmt& stmt)
{
  const auto& call = std::get<AstFunctionCall>(stmt.value);

  if (call.args[0].type == StmtType::call)
    TEST_ASSERT(check_second_assign_(ctx, call.args[0]));

  if (call.args[1].type == StmtType::call)
    TEST_ASSERT(check_second_assign_(ctx, call.args[1]));

  if (call.name == STDOP_FUNC_STR[underlay_cast(TOKENID::OP_MUL)]) {
    TEST_ASSERT(call.args.size() == 2);

    bool arg_12 = false, arg_13 = false;

    for (const auto& arg : call.args) {
      TEST_ASSERT(arg.type == StmtType::varref);

      arg_12 |= std::get<AstSymRef>(arg.value).name == "12";
      arg_13 |= std::get<AstSymRef>(arg.value).name == "13";
    }

    TEST_ASSERT(arg_12);
    TEST_ASSERT(arg_13);

    ++ctx.second_assignment.muls_checked;
  }

  if (call.name == STDOP_FUNC_STR[underlay_cast(TOKENID::OP_PLUS)]) {
    TEST_ASSERT(call.args.size() == 2);

    for (const auto& arg : call.args) {
      if (arg.type != StmtType::varref)
        continue;

      TEST_ASSERT(std::get<AstSymRef>(call.args[0].value).name == "4");
    }

    ++ctx.second_assignment.adds_checked;
  }

  return true;
}

static bool
check_second_assign(struct test_ctx& ctx, const AstStmt& stmt)
{
  check_second_assign_(ctx, stmt);

  TEST_ASSERT(ctx.second_assignment.adds_checked == 1);
  TEST_ASSERT(ctx.second_assignment.muls_checked == 1);

  return true;
}

bool
operator_precedence_test()
{
  struct test_ctx ctx;
  Tokenizer       tokenizer(testsrc, sizeof(testsrc) - 1);
  Parser          parser(tokenizer);
  AST             ast = parser.buildAST();

  auto* func_node = ast.root.nodes[0].get();
  TEST_ASSERT(func_node->id == ASTID::funcdecl);

  auto& func = std::get<AstFunction>(func_node->value);
  TEST_ASSERT(func.name == "func");
  TEST_ASSERT(func.return_type == LangType::lt_void);
  TEST_ASSERT(func.args.size() == 0);

  TEST_ASSERT(func_node->nodes[0]->id == ASTID::vardecl);
  TEST_ASSERT(func_node->nodes[1]->id == ASTID::vardecl);
  TEST_ASSERT(func_node->nodes[2]->id == ASTID::vardecl);


  //
  // Check AST for first assignment
  //
  const auto *stmt_node = func_node->nodes[3].get();
  TEST_ASSERT(stmt_node->id == ASTID::stmt);

  const auto* stmt = &std::get<AstStmt>(stmt_node->value);
  TEST_ASSERT(stmt->type == StmtType::call);

  auto* call = &std::get<AstFunctionCall>(stmt->value);
  TEST_ASSERT(call->name == STDOP_FUNC_STR[underlay_cast(TOKENID::OP_EQ)]);
  TEST_ASSERT(call->args[0].type == StmtType::varref);
  TEST_ASSERT(std::get<AstSymRef>(call->args[0].value).name == "a");

  TEST_ASSERT(check_first_assign(ctx, *stmt));


  //
  // Check AST for second assignment
  //
  stmt_node = func_node->nodes[4].get();
  TEST_ASSERT(stmt_node->id == ASTID::stmt);

  stmt = &std::get<AstStmt>(stmt_node->value);
  TEST_ASSERT(stmt->type == StmtType::call);

  call = &std::get<AstFunctionCall>(stmt->value);
  TEST_ASSERT(call->name == STDOP_FUNC_STR[underlay_cast(TOKENID::OP_EQ)]);
  TEST_ASSERT(call->args[0].type == StmtType::varref);
  TEST_ASSERT(std::get<AstSymRef>(call->args[0].value).name == "b");

  TEST_ASSERT(check_second_assign(ctx, *stmt));

  return true;
}
