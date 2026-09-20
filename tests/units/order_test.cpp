#include <cassert>
#include <type_traits>

#include "quant/execution/order_request.hpp"
#include "quant/execution/order_state.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::execution;

  OrderRequest request{.instrument_id = InstrumentId{1},
                       .side = Side::Buy,
                       .type = OrderType::Limit,
                       .time_in_force = TimeInForce::Day,
                       .quantity = Quantity{100},
                       .price = Price{10005}};

  assert(request.instrument_id.value == 1);
  assert(request.side == Side::Buy);
  assert(request.type == OrderType::Limit);
  assert(request.time_in_force == TimeInForce::Day);
  assert(request.quantity.value == 100);
  assert(request.price.value == 10005);

  OrderState state{.order_id = OrderId{42},
                   .status = OrderStatus::New,
                   .requested_quantity = Quantity{100},
                   .filled_quantity = Quantity{0},
                   .average_fill_price = Price{0}};

  assert(state.order_id.value == 42);
  assert(state.status == OrderStatus::New);
  assert(state.requested_quantity.value == 100);
  assert(state.filled_quantity.value == 0);

  static_assert(std::is_trivially_copyable_v<OrderRequest>);
  static_assert(std::is_standard_layout_v<OrderRequest>);

  static_assert(std::is_trivially_copyable_v<OrderState>);
  static_assert(std::is_standard_layout_v<OrderState>);

  return 0;
}