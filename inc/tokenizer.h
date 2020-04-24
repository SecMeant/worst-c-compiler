#pragma once

#include <string>

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

  void consume_ws();

  Token get();
  Token peek() { return Tokenizer(*this).get(); }
};

} // namespace wcc
