#pragma once

#include <fmt/format.h>

#include <mipc/utils.h>

#include "token.h"

template<>
struct fmt::formatter<wcc::Token>
{

  constexpr auto parse(format_parse_context& ctx)
  {
    const auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it != '}')
      throw format_error("invalid format");
    return it;
  }

  template<typename FormatContext>
  auto format(const wcc::Token& t, FormatContext& ctx)
  {

    return t.id == wcc::TOKENID::IDENTIFIER
             ? format_to(ctx.out(),
                         "Token: {} \"{}\"",
                         wcc::TOKENID_STR[mipc::utils::underlay_cast(t.id)],
                         t.value)
             : format_to(ctx.out(),
                         "Token: {}",
                         wcc::TOKENID_STR[mipc::utils::underlay_cast(t.id)]);
  }
};
