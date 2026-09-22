#include <cassert>

#include "quant/execution/order_manager.hpp"

int main() {
  using namespace quant::execution;

  OrderManager om;
  assert(om.active_order_count() == 0);

  OrderRequest req{
      .instrument_id = {101},
      .side = Side::Buy,
      .type = OrderType::Limit,
      .time_in_force = TimeInForce::Day,
      .quantity = {100},
      .price = {500}};

  om.track_order(req, quant::core::OrderId{1});
  assert(om.active_order_count() == 1);
  assert(om.is_active(quant::core::OrderId{1}));

  OrderState state{};
  assert(om.get_order_state(quant::core::OrderId{1}, state));
  assert(state.status == OrderStatus::New);
  assert(state.requested_quantity.value == 100);
  assert(state.filled_quantity.value == 0);

  Fill partial_fill{
      .order_id = {1},
      .execution_id = {1},
      .timestamp = {},
      .price = {500},
      .quantity = {40}};

  om.on_fill(partial_fill);
  assert(om.get_order_state(quant::core::OrderId{1}, state));
  assert(state.status == OrderStatus::PartiallyFilled);
  assert(state.filled_quantity.value == 40);
  assert(om.is_active(quant::core::OrderId{1}));

  Fill final_fill{
      .order_id = {1},
      .execution_id = {2},
      .timestamp = {},
      .price = {500},
      .quantity = {60}};

  om.on_fill(final_fill);
  assert(om.get_order_state(quant::core::OrderId{1}, state));
  assert(state.status == OrderStatus::Filled);
  assert(state.filled_quantity.value == 100);
  assert(!om.is_active(quant::core::OrderId{1}));
  assert(om.active_order_count() == 0);

  return 0;
}
