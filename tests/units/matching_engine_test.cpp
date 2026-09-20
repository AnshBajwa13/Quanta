#include <cassert>

#include "quant/execution/matching_engine.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::execution;
  using namespace quant::market;

  const MarketTick tick{.instrument_id = InstrumentId{1},
                        .timestamp = Timestamp{Duration{123456789}},
                        .bid_price = Price{9995},
                        .bid_quantity = Quantity{100},
                        .ask_price = Price{10000},
                        .ask_quantity = Quantity{50}};

  const OrderRequest buy{.instrument_id = InstrumentId{1},
                         .side = Side::Buy,
                         .type = OrderType::Limit,
                         .time_in_force = TimeInForce::Day,
                         .quantity = Quantity{100},
                         .price = Price{10000}};

  Fill fill{};

  const bool matched =
      MatchingEngine::match(buy, tick, OrderId{1}, ExecutionId{1}, fill);

  assert(matched);
  assert(fill.order_id.value == 1);
  assert(fill.execution_id.value == 1);
  assert(fill.price.value == 10000);
  assert(fill.quantity.value == 50);

  const OrderRequest non_marketable_buy{.instrument_id = InstrumentId{1},
                                        .side = Side::Buy,
                                        .type = OrderType::Limit,
                                        .time_in_force = TimeInForce::Day,
                                        .quantity = Quantity{100},
                                        .price = Price{9990}};

  Fill no_fill{};

  assert(!MatchingEngine::match(non_marketable_buy, tick, OrderId{2},
                                ExecutionId{2}, no_fill));

  const OrderRequest sell{.instrument_id = InstrumentId{1},
                          .side = Side::Sell,
                          .type = OrderType::Limit,
                          .time_in_force = TimeInForce::Day,
                          .quantity = Quantity{25},
                          .price = Price{9995}};

  Fill sell_fill{};

  assert(
      MatchingEngine::match(sell, tick, OrderId{3}, ExecutionId{3}, sell_fill));

  assert(sell_fill.price.value == 9995);
  assert(sell_fill.quantity.value == 25);

  return 0;
}