#pragma once

#include <cstdint>
#include <vector>

#include "quant/execution/matching_engine.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/execution/order_state.hpp"
#include "quant/market/market_tick.hpp"

namespace quant::execution {

class Simulator {
public:
  void reserve(std::size_t capacity);

  core::OrderId submit(const OrderRequest &request);

  bool cancel(core::OrderId order_id) noexcept;

  void on_market_tick(const market::MarketTick &tick) noexcept;

  bool get_order_state(core::OrderId order_id,
                       OrderState &state) const noexcept;

private:
  struct ActiveOrder {
    OrderRequest request{};
    OrderState state{};
    bool active{};
  };

  std::vector<ActiveOrder> orders_;

  std::uint64_t next_order_id_{1};
  std::uint64_t next_execution_id_{1};
};

} // namespace quant::execution