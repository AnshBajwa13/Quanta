#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "quant/core/ids.hpp"
#include "quant/execution/execution_listener.hpp"
#include "quant/execution/execution_report.hpp"
#include "quant/execution/fill.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/execution/order_state.hpp"

namespace quant::execution {

class OrderManager : public IExecutionListener {
public:
  OrderManager() = default;
  ~OrderManager() override = default;

  void reserve(std::size_t capacity);

  void track_order(const OrderRequest &request, core::OrderId order_id);

  void on_execution_report(const ExecutionReport &report) noexcept override;
  void on_fill(const Fill &fill) noexcept override;

  [[nodiscard]] bool get_order_state(core::OrderId order_id,
                                     OrderState &state) const noexcept;

  [[nodiscard]] bool is_active(core::OrderId order_id) const noexcept;
  [[nodiscard]] std::size_t active_order_count() const noexcept;

private:
  struct ManagedOrder {
    OrderRequest request{};
    OrderState state{};
    bool active{true};
  };

  std::vector<ManagedOrder> orders_;
  std::unordered_map<std::uint64_t, std::size_t> order_index_;
  std::size_t active_orders_count_{0};
};

} // namespace quant::execution
