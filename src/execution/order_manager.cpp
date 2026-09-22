#include "quant/execution/order_manager.hpp"

namespace quant::execution {

void OrderManager::track_order(const OrderRequest &request, core::OrderId order_id) {
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
}

void OrderManager::on_execution_report(const ExecutionReport &report) noexcept {
  for (auto &order : orders_) {
    if (order.state.order_id.value == report.order_id.value) {
      order.state.status = report.status;
      order.state.filled_quantity = report.cumulative_quantity;
      order.state.average_fill_price = report.average_price;

      if (report.status == OrderStatus::Filled ||
          report.status == OrderStatus::Cancelled ||
          report.status == OrderStatus::Rejected ||
          report.status == OrderStatus::Expired) {
        order.active = false;
      }
      return;
    }
  }
}

void OrderManager::on_fill(const Fill &fill) noexcept {
  for (auto &order : orders_) {
    if (order.state.order_id.value == fill.order_id.value) {
      order.state.filled_quantity.value += fill.quantity.value;
      if (order.state.filled_quantity.value >= order.request.quantity.value) {
        order.state.status = OrderStatus::Filled;
        order.active = false;
      } else {
        order.state.status = OrderStatus::PartiallyFilled;
      }
      return;
    }
  }
}

bool OrderManager::get_order_state(core::OrderId order_id,
                                   OrderState &state) const noexcept {
  for (const auto &order : orders_) {
    if (order.state.order_id.value == order_id.value) {
      state = order.state;
      return true;
    }
  }
  return false;
}

bool OrderManager::is_active(core::OrderId order_id) const noexcept {
  for (const auto &order : orders_) {
    if (order.state.order_id.value == order_id.value) {
      return order.active;
    }
  }
  return false;
}

std::size_t OrderManager::active_order_count() const noexcept {
  std::size_t count{0};
  for (const auto &order : orders_) {
    if (order.active) {
      ++count;
    }
  }
  return count;
}

} // namespace quant::execution
