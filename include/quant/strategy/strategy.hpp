#pragma once

#include <concepts>

#include "quant/core/ids.hpp"
#include "quant/execution/fill.hpp"
#include "quant/execution/order_state.hpp"
#include "quant/market/market_tick.hpp"
#include "quant/market/trade.hpp"

namespace quant::strategy {

template <typename T>
concept Strategy =
    requires(T &strategy, const T &cstrategy, const market::MarketTick &tick,
             const market::Trade &trade, const execution::OrderState &state,
             const execution::Fill &fill) {
      { cstrategy.strategy_id() } noexcept -> std::same_as<core::StrategyId>;
      { strategy.on_market_tick(tick) } noexcept -> std::same_as<void>;
      { strategy.on_trade(trade) } noexcept -> std::same_as<void>;
      { strategy.on_order_state(state) } noexcept -> std::same_as<void>;
      { strategy.on_fill(fill) } noexcept -> std::same_as<void>;
    };

class IStrategy {
public:
  virtual ~IStrategy() = default;

  [[nodiscard]] virtual core::StrategyId strategy_id() const noexcept = 0;
  virtual void on_market_tick(const market::MarketTick &tick) noexcept = 0;
  virtual void on_trade(const market::Trade &trade) noexcept = 0;
  virtual void on_order_state(const execution::OrderState &state) noexcept = 0;
  virtual void on_fill(const execution::Fill &fill) noexcept = 0;
};

} // namespace quant::strategy
