#include <cassert>
#include <type_traits>

#include "quant/execution/fill.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::execution;

  Fill fill{.order_id = OrderId{42},
            .execution_id = ExecutionId{1001},
            .timestamp = Timestamp{Duration{123456789}},
            .price = Price{10005},
            .quantity = Quantity{50}};

  assert(fill.order_id.value == 42);
  assert(fill.execution_id.value == 1001);
  assert(fill.timestamp.value.count() == 123456789);
  assert(fill.price.value == 10005);
  assert(fill.quantity.value == 50);

  static_assert(std::is_trivially_copyable_v<Fill>);
  static_assert(std::is_standard_layout_v<Fill>);

  static_assert(sizeof(Fill) <= 40);

  return 0;
}