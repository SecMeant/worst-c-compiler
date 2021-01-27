#include "parser.h"
#include "ast.h"
#include "token.h"
#include "ast_format.h"

#include <spdlog/spdlog.h>
#include <variant>

namespace wcc {

static void
syntax_error(const char* got_before,
             const char* expected,
             const char* got_after,
             size_t      line,
             size_t      pos)
{
  spdlog::error("Syntax error: Expected {} after {}, but got {}",
                expected,
                got_before,
                got_after);
  spdlog::error("At: {}:{}", line, pos);
}

static bool
parse_str_field(Tokenizer& tokenizer, AstVariable& field)
{
  Token token = tokenizer.get();

  if (token.id != TOKENID::IDENTIFIER) {
    spdlog::error("Syntax error: Expected structure field declaration (type "
                  "identifier), but got {}",
                  TOKENID_STR[underlay_cast(token.id)]);
    spdlog::error("At: {}:{}", token.line, token.pos);
    return false;
  }

  std::optional type_opt = lookup_type(token.value);
  if (!type_opt.has_value()) {
    spdlog::error("Syntax error: Expected structure field declaration (type "
                  "identifier), unkown type \"{}\"",
                  token.value);
    spdlog::error("At: {}:{}", token.line, token.pos);
    return false;
  }

  field.type = *type_opt;

  token = tokenizer.get();
  if (token.id != TOKENID::IDENTIFIER) {
    spdlog::error(
      "Syntax error: Expected structure field name (identifier), but got {}",
      TOKENID_STR[underlay_cast(token.id)]);
    spdlog::error("At: {}:{}", token.line, token.pos);
    return false;
  }

  field.name = token.value;

  token = tokenizer.get();
  if (token.id != TOKENID::SEMICOLON) {
    spdlog::error("Syntax error: Expected semicolon after structure field "
                  "declaration. but got {}",
                  TOKENID_STR[underlay_cast(token.id)]);
    spdlog::error("At: {}:{}", token.line, token.pos);
    return false;
  }

  spdlog::debug("Parsed structure field: Type:{}, Name:{}",
                LANG_TYPE_STR[underlay_cast(field.type)],
                field.name);

  return true;
}

static bool
parse_strdecl(Tokenizer& tokenizer, AstStruct& str)
{
  while (1) {
    if (tokenizer.peek().id == TOKENID::BLOCK_END) {
      tokenizer.get();
      if (const auto token = tokenizer.get(); token.id != TOKENID::SEMICOLON) {
        spdlog::error("Syntax error: Structure declaration must end with "
                      "semicolon, but got: {}",
                      TOKENID_STR[underlay_cast(token.id)]);
        return false;
      }
      break;
    }

    str.fields.emplace_back();
    AstVariable& field = str.fields.back();
    const auto   res   = parse_str_field(tokenizer, field);

    if (!res)
      return false;
  }

  return true;
}

static bool
parse_func_params(Tokenizer& tokenizer, ASTNode& node)
{
  Token token;

  while (1) {
    ASTNode& param = node.add(ASTID::vardecl);
    param.value    = AstVariable();

    token = tokenizer.get();

    if (token.id != TOKENID::IDENTIFIER) {
      spdlog::error("Syntax error: Expected type name, got {}",
                    TOKENID_STR[underlay_cast(token.id)]);
      spdlog::error("At: {}:{}", token.line, token.pos);
      return false;
    }

    std::optional type = lookup_type(token.value);

    if (!type.has_value()) {
      spdlog::error("Unknown type \"{}\"", token.value);
      spdlog::error("At: {}:{}", token.line, token.pos);
      return false;
    }

    token = tokenizer.get();

    AstVariable& var = std::get<AstVariable>(param.value);
    var.type         = type.value();

    if (token.id == TOKENID::IDENTIFIER) {
      var.name = token.value;
      token    = tokenizer.get();
    }

    spdlog::debug("Parsed func param type: {}, name: {}",
                  LANG_TYPE_STR[underlay_cast(var.type)],
                  var.name);

    if (token.id == TOKENID::COMMA)
      continue;

    if (token.id != TOKENID::PAREN_CLOSE) {
      spdlog::error("Syntax error: Expected closing parenthesis ')', got {}",
                    TOKENID_STR[underlay_cast(token.id)]);
      spdlog::error("At: {}:{}", token.line, token.pos);
      return false;
    }

    break;
  }

  return true;
}
static bool
parse_code_block(Tokenizer& tokenizer, ASTNode& node);

static bool
parse_funcdecl(Tokenizer& tokenizer, ASTNode& node)
{
  AstFunction& func  = std::get<AstFunction>(node.value);
  Token        token = tokenizer.get();

  if (token.id != TOKENID::PAREN_OPEN) {
    spdlog::error("Syntax error: Expected argument list, got {}",
                  TOKENID_STR[underlay_cast(token.id)]);
    spdlog::error("At: {}:{}", token.line, token.pos);
    return false;
  }

  if (tokenizer.peek().id == TOKENID::PAREN_CLOSE) {
    tokenizer.get();
  } else {
    const auto parse_res = parse_func_params(tokenizer, node);

    if (!parse_res)
      return false;
  }

  token = tokenizer.get();
  if (token.id != TOKENID::BLOCK_BEGIN) {
    spdlog::error(
      "Syntax error: Expected function body after header, but got {}",
      TOKENID_STR[underlay_cast(token.id)]);
    return false;
  }

  return parse_code_block(tokenizer, node);
}

static bool
is_stdop(const Token token)
{
  switch (token.id) {
    case TOKENID::OP_NEG:
    case TOKENID::OP_NEQ:
    case TOKENID::OP_MOD:
    case TOKENID::OP_XOR:
    case TOKENID::OP_LOGIC_AND:
    case TOKENID::OP_ANDEQ:
    case TOKENID::OP_AND:
    case TOKENID::OP_LOGIC_OR:
    case TOKENID::OP_OREQ:
    case TOKENID::OP_OR:
    case TOKENID::OP_MUL:
    case TOKENID::OP_MULEQ:
    case TOKENID::OP_DIVEQ:
    case TOKENID::OP_DIV:
    case TOKENID::OP_MINUS:
    case TOKENID::OP_ACCESS:
    case TOKENID::OP_PLUS:
    case TOKENID::OP_EQ:
    case TOKENID::OP_LS:
    case TOKENID::OP_LSE:
    case TOKENID::OP_GR:
    case TOKENID::OP_GRE:
    case TOKENID::OP_DOT:
      return true;

    default:
      return false;
  }

  return false;
}

static bool
is_function_call(Token token)
{
  return is_stdop(token) || token.id == TOKENID::PAREN_OPEN;
}

static bool
is_at_function_call(const Tokenizer& t)
{
  return is_function_call(t.peek());
}

static std::optional<std::string>
get_function_symbol_name(Token t)
{
  if (is_stdop(t))
    return STDOP_FUNC_STR[underlay_cast(t.id)];

  if (t.id != TOKENID::IDENTIFIER)
    return std::nullopt;

  return t.value;
}

static bool
parse_statement_list(Tokenizer& tokenizer, ASTNode& node)
{
  if (!(node.id == ASTID::stmt &&
        std::holds_alternative<AstStmt>(node.value))) {
    spdlog::critical("Internal error: Expected AstFunctionCall node as "
                     "argument in parse_statement_list, but got {}",
                     node);
    return false;
  }

  AstStmt& stmt_node = std::get<AstStmt>(node.value);

  if (stmt_node.type != StmtType::call) {
    spdlog::critical("Internal error: Expected AstFunctionCall node as "
                     "argument in parse_statement_list, but got {}",
                     node);
    return false;
  }

  AstFunctionCall& call =
    std::get<AstFunctionCall>(std::get<AstStmt>(node.value).value);

  Token tok = tokenizer.peek();

  if (tok.id == TOKENID::PAREN_CLOSE)
    return true;

  while (1) {
    tok = tokenizer.get();

    if (tok.id != TOKENID::IDENTIFIER) {
      spdlog::error("Syntax error: expected argument identifier in call "
                    "parenthesis, but got {}",
                    TOKENID_STR[underlay_cast(tok.id)]);
      spdlog::error("At: {}:{}", tok.line, tok.pos);
      return false;
    }

    call.args.emplace_back(AstSymRef{ .name = tok.value });

    tok = tokenizer.get();

    if (tok.id == TOKENID::PAREN_CLOSE)
      break;

    if (tok.id != TOKENID::COMMA) {
      spdlog::error("Syntax error: expected comma separated list of arguments");
      spdlog::error("At: {}:{}", tok.line, tok.pos);
      return false;
    }
  }

  return true;
}

static std::optional<SymbolName>
parse_statement_symbol(Tokenizer& tokenizer, ASTNode& node)
{
  Token symtok = tokenizer.get();
  if (symtok.id != TOKENID::IDENTIFIER)
    return std::nullopt;

  return SymbolName(symtok.value);
}

static bool
parse_statement(Tokenizer& tokenizer, ASTNode& node)
{
  ASTNode& stmt_node = node.add(ASTID::stmt);
  stmt_node.value    = AstStmt();
  AstStmt& stmt      = std::get<AstStmt>(stmt_node.value);

  std::optional<SymbolName> opt_sym;
  if (opt_sym = parse_statement_symbol(tokenizer, node); !opt_sym.has_value()) {
    return false;
  }

  if (tokenizer.peek().id == TOKENID::PAREN_OPEN) {
    tokenizer.get();

    stmt.type  = StmtType::call;
    stmt.value = AstFunctionCall{ .name = opt_sym.value() };

    if (!parse_statement_list(tokenizer, stmt_node))
      return false;
  } else {
    stmt.type  = StmtType::varref;
    stmt.value = AstSymRef{ .name = opt_sym.value() };
  }

  if (is_stdop(tokenizer.peek())) {
    Token optok = tokenizer.get();

    ASTNode opnode(ASTID::stmt);
    opnode.value = AstStmt();

    AstStmt& stmt_rhs = std::get<AstStmt>(opnode.value);
    stmt_rhs.type     = StmtType::call;
    stmt_rhs.value    = AstFunctionCall();

    AstFunctionCall& astcall = std::get<AstFunctionCall>(stmt_rhs.value);
    astcall.name             = STDOP_FUNC_STR[underlay_cast(optok.id)];

    // Rotate tree because we found stdop after func call
    ASTNode& stmt_node_reloc = opnode.add(ASTID::stmt);
    stmt_node_reloc          = std::move(stmt_node);
    stmt_node                = std::move(opnode);

    if (tokenizer.peek().id != TOKENID::SEMICOLON)
      return parse_statement(tokenizer, stmt_node);
  }

  if (tokenizer.peek().id == TOKENID::SEMICOLON)
    tokenizer.get();

  return true;
}

static bool
parse_code_block(Tokenizer& tokenizer, ASTNode& node)
{
  Token token;

  while (1) {

    // TODO: This is temporary hack to make parse_statement start from the
    // begining of the line.
    Tokenizer beginState = tokenizer;

    token = tokenizer.get();

    switch (token.id) {

      case TOKENID::BLOCK_END:
        return true;

      case TOKENID::IDENTIFIER: {

        if (token.value == "struct") {

          token = tokenizer.get();

          if (token.id != TOKENID::IDENTIFIER) {
            syntax_error("struct",
                         "identifier",
                         TOKENID_STR[underlay_cast(token.id)],
                         token.line,
                         token.pos);
            return false;
          }

          Token next_token = tokenizer.get();

          if (next_token.id == TOKENID::BLOCK_BEGIN) {
            ASTNode& str_node = node.add(ASTID::strdecl);
            str_node.value    = AstStruct();
            AstStruct& str    = std::get<AstStruct>(str_node.value);

            str.name = token.value;

            return parse_strdecl(tokenizer, str);
          }

          syntax_error("struct",
                       "identifier",
                       TOKENID_STR[underlay_cast(token.id)],
                       token.line,
                       token.pos);
        }

        else if (const auto opt_type = lookup_type(token.value);
                 opt_type.has_value()) {

          Token symbol_name = tokenizer.get();
          if (symbol_name.id != TOKENID::IDENTIFIER) {
            spdlog::error("Syntax error: Expected variable or function name "
                          "after type identifier {}",
                          token.value);
            spdlog::error("At: {}:{}", token.line, token.pos);

            return false;
          }

          if (tokenizer.peek().id == TOKENID::PAREN_OPEN) {
            ASTNode& func_node = node.add(ASTID::funcdecl);
            func_node.value    = AstFunction();
            AstFunction& func  = std::get<AstFunction>(func_node.value);

            func.return_type = opt_type.value();
            func.name        = symbol_name.value;

            return parse_funcdecl(tokenizer, func_node);
          }

          else if (tokenizer.peek().id == TOKENID::SEMICOLON) {
            ASTNode& vardecl_node = node.add(ASTID::vardecl);
            vardecl_node.value    = AstVariable();
            AstVariable& vardecl  = std::get<AstVariable>(vardecl_node.value);

            vardecl.type = opt_type.value();
            vardecl.name = symbol_name.value;

            spdlog::debug("Parsed variable declaration: {}", vardecl.name);
            tokenizer.get();
            continue;
          }

          spdlog::error(
            "Syntax error: Expected function or variable declaration.");
          spdlog::error("At: {}:{}", token.line, token.pos);
          return false;
        }

        tokenizer = beginState;
        if (!parse_statement(tokenizer, node))
          return false;

        continue;
      }

      default:
        spdlog::error(
          "Unsupported token in code block at {}:{}", token.line, token.pos);
        return false;
    }
  }

  return true;
}

AST
Parser::buildAST()
{
  AST      ast;
  ASTNode& ast_root = ast.root;

  while (1) {

    Token token;
    token = tokenizer.peek();

    spdlog::debug("Token: {}", ASTID_STR[underlay_cast(token.id)]);

    if (token.id == TOKENID::END)
      break;

    const auto parse_res = parse_code_block(this->tokenizer, ast_root);

    if (!parse_res)
      break;
  }

  return ast;
}
}
