#include "quant/strategy/strategy_engine.hpp"

#include <algorithm>

namespace quant::strategy {

void StrategyEngine::register_strategy(IStrategy *strategy) {
  if (strategy != nullptr) {
    strategies_.push_back(strategy);
  }
}

void StrategyEngine::unregister_strategy(core::StrategyId strategy_id) {
  strategies_.erase(
      std::remove_if(strategies_.begin(), strategies_.end(),
                     [strategy_id](const IStrategy *s) {
                       return s != nullptr && s->strategy_id().value == strategy_id.value;
                     }),
      strategies_.end());
}

void StrategyEngine::on_market_tick(const market::MarketTick &tick) noexcept {
  for (auto *strategy : strategies_) {
    if (strategy != nullptr) {
      strategy->on_market_tick(tick);
    }
  }
}

void StrategyEngine::on_trade(const market::Trade &trade) noexcept {
  for (auto *strategy : strategies_) {
    if (strategy != nullptr) {
      strategy->on_trade(trade);
    }
  }
}

void StrategyEngine::on_order_state(const execution::OrderState &state) noexcept {
  for (auto *strategy : strategies_) {
    if (strategy != nullptr) {
      strategy->on_order_state(state);
    }
  }
}

void StrategyEngine::on_fill(const execution::Fill &fill) noexcept {
  for (auto *strategy : strategies_) {
    if (strategy != nullptr) {
      strategy->on_fill(fill);
    }
  }
}

std::size_t StrategyEngine::strategy_count() const noexcept {
  return strategies_.size();
}

} // namespace quant::strategy
