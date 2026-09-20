#include <cassert>
#include <type_traits>

#include "quant/market/trade.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::market;

  Trade trade{.instrument_id = InstrumentId{1},
              .timestamp = Timestamp{Duration{123456789}},
              .price = Price{10005},
              .quantity = Quantity{100}};

  assert(trade.instrument_id.value == 1);
  assert(trade.timestamp.value.count() == 123456789);
  assert(trade.price.value == 10005);
  assert(trade.quantity.value == 100);

  static_assert(std::is_trivially_copyable_v<Trade>);
  static_assert(std::is_standard_layout_v<Trade>);
  static_assert(sizeof(Trade) <= 32);

  return 0;
}