#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "quant/execution/execution_listener.hpp"
#include "quant/execution/matching_engine.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/execution/order_state.hpp"
#include "quant/market/market_tick.hpp"

namespace quant::execution {

class Simulator {
public:
  Simulator() = default;
  ~Simulator() = default;

  void reserve(std::size_t capacity);

  void set_listener(IExecutionListener *listener) noexcept {
    listener_ = listener;
  }

  core::OrderId submit(const OrderRequest &request);

  bool cancel(core::OrderId order_id) noexcept;

  void on_market_tick(const market::MarketTick &tick) noexcept;

  [[nodiscard]] bool get_order_state(core::OrderId order_id,
                                     OrderState &state) const noexcept;

  [[nodiscard]] std::size_t active_order_count() const noexcept;

private:
  struct ActiveOrder {
    OrderRequest request{};
    OrderState state{};
    bool active{false};
  };

  std::vector<ActiveOrder> orders_;
  std::vector<std::size_t> active_indices_;
  IExecutionListener *listener_{nullptr};

  std::uint64_t next_order_id_{1};
  std::uint64_t next_execution_id_{1};
};

} // namespace quant::execution