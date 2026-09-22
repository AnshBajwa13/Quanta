#include "quant/execution/order_manager.hpp"

namespace quant::execution {

void OrderManager::reserve(std::size_t capacity) {
  orders_.reserve(capacity);
  order_index_.reserve(capacity);
}

void OrderManager::track_order(const OrderRequest &request, core::OrderId order_id) {
  const auto it = order_index_.find(order_id.value);
  if (it != order_index_.end()) {
    auto &order = orders_[it->second];
    order.request = request;
    order.state = {
        .order_id = order_id,
        .status = OrderStatus::New,
        .requested_quantity = request.quantity,
        .filled_quantity = core::Quantity{0},
        .average_fill_price = core::Price{0}};
    if (!order.active) {
      order.active = true;
      ++active_orders_count_;
    }
    return;
  }

  const std::size_t index = orders_.size();
  ManagedOrder managed{
      .request = request,
      .state = {
          .order_id = order_id,
          .status = OrderStatus::New,
          .requested_quantity = request.quantity,
          .filled_quantity = core::Quantity{0},
          .average_fill_price = core::Price{0}},
      .active = true};
  orders_.push_back(managed);
  order_index_[order_id.value] = index;
  ++active_orders_count_;
}

void OrderManager::on_execution_report(const ExecutionReport &report) noexcept {
  const auto it = order_index_.find(report.order_id.value);
  if (it != order_index_.end()) {
    auto &order = orders_[it->second];
    order.state.status = report.status;
    order.state.filled_quantity = report.cumulative_quantity;
    order.state.average_fill_price = report.average_price;

    if (order.active && (report.status == OrderStatus::Filled ||
                         report.status == OrderStatus::Cancelled ||
                         report.status == OrderStatus::Rejected ||
                         report.status == OrderStatus::Expired)) {
      order.active = false;
      if (active_orders_count_ > 0) {
        --active_orders_count_;
      }
    }
  }
}

void OrderManager::on_fill(const Fill &fill) noexcept {
  const auto it = order_index_.find(fill.order_id.value);
  if (it != order_index_.end()) {
    auto &order = orders_[it->second];
    order.state.filled_quantity.value += fill.quantity.value;
    if (order.state.filled_quantity.value >= order.request.quantity.value) {
      order.state.status = OrderStatus::Filled;
      if (order.active) {
        order.active = false;
        if (active_orders_count_ > 0) {
          --active_orders_count_;
        }
      }
    } else {
      order.state.status = OrderStatus::PartiallyFilled;
    }
  }
}

bool OrderManager::get_order_state(core::OrderId order_id,
                                   OrderState &state) const noexcept {
  const auto it = order_index_.find(order_id.value);
  if (it != order_index_.end()) {
    state = orders_[it->second].state;
    return true;
  }
  return false;
}

bool OrderManager::is_active(core::OrderId order_id) const noexcept {
  const auto it = order_index_.find(order_id.value);
  if (it != order_index_.end()) {
    return orders_[it->second].active;
  }
  return false;
}

std::size_t OrderManager::active_order_count() const noexcept {
  return active_orders_count_;
}

} // namespace quant::execution
