#include <cassert>
#include <type_traits>

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/time.hpp"

int main() {
  using namespace quant::core;

  OrderId order{42};
  InstrumentId instrument{1001};
  ExecutionId execution{7};
  StrategyId strategy{3};

  Quantity quantity{100};
  Price price{24535};

  Timestamp timestamp{Duration{123456789}};

  assert(order.value == 42);
  assert(instrument.value == 1001);
  assert(execution.value == 7);
  assert(strategy.value == 3);

  assert(quantity.value == 100);
  assert(price.value == 24535);
  assert(timestamp.value.count() == 123456789);

  static_assert(std::is_trivially_copyable_v<OrderId>);
  static_assert(std::is_trivially_copyable_v<InstrumentId>);
  static_assert(std::is_trivially_copyable_v<ExecutionId>);
  static_assert(std::is_trivially_copyable_v<StrategyId>);
  static_assert(std::is_trivially_copyable_v<Quantity>);
  static_assert(std::is_trivially_copyable_v<Price>);
  static_assert(std::is_trivially_copyable_v<Timestamp>);

  return 0;
}