#include "quant/execution/simulator.hpp"

namespace quant::execution {

void Simulator::reserve(std::size_t capacity) { orders_.reserve(capacity); }

core::OrderId Simulator::submit(const OrderRequest &request) {
  const core::OrderId order_id{next_order_id_++};

  ActiveOrder order{.request = request,
                    .state = OrderState{.order_id = order_id,
                                        .status = OrderStatus::Accepted,
                                        .requested_quantity = request.quantity,
                                        .filled_quantity = core::Quantity{},
                                        .average_fill_price = core::Price{}},
                    .active = true};

  orders_.push_back(order);

  return order_id;
}

void Simulator::on_market_tick(const market::MarketTick &tick) noexcept {
  for (auto &order : orders_) {
    if (!order.active) {
      continue;
    }

    if (order.state.status == OrderStatus::Filled ||
        order.state.status == OrderStatus::Cancelled ||
        order.state.status == OrderStatus::Rejected ||
        order.state.status == OrderStatus::Expired) {
      continue;
    }

    if (order.request.instrument_id.value != tick.instrument_id.value) {
      continue;
    }

    Fill fill{};

    const core::ExecutionId execution_id{next_execution_id_++};

    if (!MatchingEngine::match(order.request, tick, order.state.order_id,
                               execution_id, fill)) {
      if (order.request.time_in_force == TimeInForce::IOC ||
          order.request.time_in_force == TimeInForce::FOK) {
        order.state.status = OrderStatus::Expired;
        order.active = false;
      }
      continue;
    }

    const auto previous_filled = order.state.filled_quantity.value;
    const auto fill_quantity = fill.quantity.value;
    const auto new_filled = previous_filled + fill_quantity;

    const auto weighted_value =
        previous_filled * order.state.average_fill_price.value +
        fill_quantity * fill.price.value;

    const auto average_price = weighted_value / new_filled;

    order.state.filled_quantity = core::Quantity{new_filled};
    order.state.average_fill_price = core::Price{average_price};

    if (new_filled >= order.state.requested_quantity.value) {
      order.state.status = OrderStatus::Filled;
      order.active = false;
    } else if (order.request.time_in_force == TimeInForce::IOC) {
      order.state.status = OrderStatus::Expired;
      order.active = false;

    } else {
      order.state.status = OrderStatus::PartiallyFilled;
    }
  }
}

bool Simulator::get_order_state(core::OrderId order_id,
                                OrderState &state) const noexcept {
  for (const auto &order : orders_) {
    if (order.active && order.state.order_id.value == order_id.value) {
      state = order.state;
      return true;
    }

    // A completed order should still be queryable.
    if (order.state.order_id.value == order_id.value) {
      state = order.state;
      return true;
    }
  }

  return false;
}

bool Simulator::cancel(core::OrderId order_id) noexcept {
  for (auto &order : orders_) {
    if (order.state.order_id.value != order_id.value) {
      continue;
    }

    switch (order.state.status) {
    case OrderStatus::Accepted:
    case OrderStatus::PartiallyFilled:
      order.state.status = OrderStatus::Cancelled;
      order.active = false;
      return true;

    case OrderStatus::New:
    case OrderStatus::CancelPending:
    case OrderStatus::Filled:
    case OrderStatus::Cancelled:
    case OrderStatus::Rejected:
    case OrderStatus::Expired:
      return false;
    }
  }

  return false;
}

} // namespace quant::execution