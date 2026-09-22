#pragma once

#include <cstddef>
#include <vector>

#include "quant/core/ids.hpp"
#include "quant/execution/fill.hpp"
#include "quant/execution/order_state.hpp"
#include "quant/market/market_tick.hpp"
#include "quant/market/trade.hpp"
#include "quant/strategy/strategy.hpp"

namespace quant::strategy {

class StrategyEngine {
public:
  StrategyEngine() = default;
  ~StrategyEngine() = default;

  void register_strategy(IStrategy *strategy);
  void unregister_strategy(core::StrategyId strategy_id);

  void on_market_tick(const market::MarketTick &tick) noexcept;
  void on_trade(const market::Trade &trade) noexcept;
  void on_order_state(const execution::OrderState &state) noexcept;
  void on_fill(const execution::Fill &fill) noexcept;

  [[nodiscard]] std::size_t strategy_count() const noexcept;

private:
  std::vector<IStrategy *> strategies_;
};

} // namespace quant::strategy
