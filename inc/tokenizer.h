#pragma once

#include <string>
#include <vector>

#include "token.h"

namespace wcc {

struct Tokenizer
{
  using DataType     = TokenDataType;
  using DataViewType = const TokenDataType*;
  using DataSizeType = size_t;
  using PositionType = DataSizeType;

  DataViewType data, current, end;
  PositionType pos;
  PositionType line;

  Tokenizer(DataViewType data, DataSizeType size)
    : data(data)
    , current(data)
    , end(data + size)
    , pos(0)
    , line(0)
  {}

  Tokenizer(const Tokenizer &other) = default;

  void consume_ws();

  Token get();
  Token peek() const { return Tokenizer(*this).get(); }

  inline static std::vector<PositionType> breakpoints;
};

} // namespace wcc
