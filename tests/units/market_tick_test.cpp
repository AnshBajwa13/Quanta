#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>

#include "quant/market/market_tick.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::market;
  std::cout << "sizeof(MarketTick): " << sizeof(MarketTick) << '\n';

  std::cout << "alignof(MarketTick): " << alignof(MarketTick) << '\n';

  MarketTick tick{.instrument_id = InstrumentId{1},
                  .timestamp = Timestamp{Duration{123456789}},
                  .bid_price = Price{10000},
                  .bid_quantity = Quantity{500},
                  .ask_price = Price{10005},
                  .ask_quantity = Quantity{300}};

  assert(tick.instrument_id.value == 1);
  assert(tick.timestamp.value.count() == 123456789);

  assert(tick.bid_price.value == 10000);
  assert(tick.bid_quantity.value == 500);

  assert(tick.ask_price.value == 10005);
  assert(tick.ask_quantity.value == 300);

  static_assert(std::is_trivially_copyable_v<MarketTick>);
  static_assert(std::is_standard_layout_v<MarketTick>);

  static_assert(sizeof(MarketTick) <= 64);

  return 0;
}