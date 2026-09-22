#include "quant/execution/simulator.hpp"

namespace quant::execution {

void Simulator::reserve(std::size_t capacity) {
  orders_.reserve(capacity);
  active_indices_.reserve(capacity);
}

core::OrderId Simulator::submit(const OrderRequest &request) {
  const core::OrderId order_id{next_order_id_++};
  const std::size_t index = orders_.size();

  ActiveOrder order{.request = request,
                    .state = OrderState{.order_id = order_id,
                                        .status = OrderStatus::Accepted,
                                        .requested_quantity = request.quantity,
                                        .filled_quantity = core::Quantity{},
                                        .average_fill_price = core::Price{}},
                    .active = true};

  orders_.push_back(order);
  active_indices_.push_back(index);

  return order_id;
}

void Simulator::on_market_tick(const market::MarketTick &tick) noexcept {
  for (std::size_t i = 0; i < active_indices_.size(); /* conditional step */) {
    const std::size_t idx = active_indices_[i];
    auto &order = orders_[idx];

    if (!order.active || order.state.status == OrderStatus::Filled ||
        order.state.status == OrderStatus::Cancelled ||
        order.state.status == OrderStatus::Rejected ||
        order.state.status == OrderStatus::Expired) {
      order.active = false;
      active_indices_[i] = active_indices_.back();
      active_indices_.pop_back();
      continue;
    }

    if (order.request.instrument_id.value != tick.instrument_id.value) {
      ++i;
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
        active_indices_[i] = active_indices_.back();
        active_indices_.pop_back();
      } else {
        ++i;
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
      active_indices_[i] = active_indices_.back();
      active_indices_.pop_back();
    } else if (order.request.time_in_force == TimeInForce::IOC) {
      order.state.status = OrderStatus::Expired;
      order.active = false;
      active_indices_[i] = active_indices_.back();
      active_indices_.pop_back();
    } else {
      order.state.status = OrderStatus::PartiallyFilled;
      ++i;
    }

    if (listener_ != nullptr) {
      listener_->on_fill(fill);
      const ExecutionReport report{
          .order_id = fill.order_id,
          .execution_id = fill.execution_id,
          .timestamp = fill.timestamp,
          .status = order.state.status,
          .side = order.request.side,
          .last_price = fill.price,
          .last_quantity = fill.quantity,
          .cumulative_quantity = order.state.filled_quantity,
          .leaves_quantity = core::Quantity{order.state.requested_quantity.value - order.state.filled_quantity.value},
          .average_price = order.state.average_fill_price,
          .text = {}};
      listener_->on_execution_report(report);
    }
  }
}

bool Simulator::get_order_state(core::OrderId order_id,
                                OrderState &state) const noexcept {
  if (order_id.value >= 1 && order_id.value <= orders_.size()) {
    state = orders_[order_id.value - 1].state;
    return true;
  }
  return false;
}

bool Simulator::cancel(core::OrderId order_id) noexcept {
  if (order_id.value >= 1 && order_id.value <= orders_.size()) {
    auto &order = orders_[order_id.value - 1];
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

std::size_t Simulator::active_order_count() const noexcept {
  return active_indices_.size();
}

} // namespace quant::execution