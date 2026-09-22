#include <cassert>

#include "quant/strategy/signal.hpp"
#include "quant/strategy/strategy.hpp"
#include "quant/strategy/strategy_engine.hpp"

class DummyStrategy : public quant::strategy::IStrategy {
public:
  explicit DummyStrategy(quant::core::StrategyId id) : id_(id) {}

  [[nodiscard]] quant::core::StrategyId strategy_id() const noexcept override {
    return id_;
  }

  void on_market_tick(const quant::market::MarketTick &) noexcept override {
    tick_count++;
  }
  void on_trade(const quant::market::Trade &) noexcept override {}
  void on_order_state(const quant::execution::OrderState &) noexcept override {}
  void on_fill(const quant::execution::Fill &) noexcept override {
    fill_count++;
  }

  quant::core::StrategyId id_;
  int tick_count{0};
  int fill_count{0};
};

int main() {
  using namespace quant::strategy;

  DummyStrategy strat(quant::core::StrategyId{1});
  StrategyEngine engine;

  assert(engine.strategy_count() == 0);
  engine.register_strategy(&strat);
  assert(engine.strategy_count() == 1);

  quant::market::MarketTick tick{};
  engine.on_market_tick(tick);
  assert(strat.tick_count == 1);

  quant::execution::Fill fill{};
  engine.on_fill(fill);
  assert(strat.fill_count == 1);

  engine.unregister_strategy(quant::core::StrategyId{1});
  assert(engine.strategy_count() == 0);

  return 0;
}
