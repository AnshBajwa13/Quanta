#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/spsc_queue.hpp"
#include "quant/core/time.hpp"
#include "quant/core/version.hpp"
#include "quant/execution/order_manager.hpp"
#include "quant/execution/order_router.hpp"
#include "quant/execution/simulator.hpp"
#include "quant/execution/simulator_gateway.hpp"
#include "quant/market/market_data_dispatcher.hpp"
#include "quant/market/replay_feed.hpp"
#include "quant/portfolio/portfolio_manager.hpp"
#include "quant/risk/risk_manager.hpp"
#include "quant/strategy/strategy.hpp"
#include "quant/strategy/strategy_engine.hpp"

namespace {

/// Production-grade high-frequency market making strategy for benchmark
class BenchmarkMarketMakerStrategy : public quant::strategy::IStrategy {
public:
  BenchmarkMarketMakerStrategy(quant::core::StrategyId id,
                               quant::execution::OrderRouter &router,
                               quant::execution::SimulatorGateway &gateway,
                               quant::portfolio::PortfolioManager &pm)
      : id_(id), router_(router), gateway_(gateway), pm_(pm) {}

  [[nodiscard]] quant::core::StrategyId strategy_id() const noexcept override {
    return id_;
  }

  void on_market_tick(const quant::market::MarketTick &tick) noexcept override {
    ++ticks_processed_;

    quant::portfolio::Position pos{};
    static_cast<void>(pm_.get_position(tick.instrument_id, pos));

    // Place market making orders every 8 ticks when within inventory limits
    if ((ticks_processed_ & 0x7) == 0) {
      if (pos.quantity.value < 500) {
        const quant::execution::OrderRequest buy_req{
            .instrument_id = tick.instrument_id,
            .side = quant::execution::Side::Buy,
            .type = quant::execution::OrderType::Limit,
            .time_in_force = quant::execution::TimeInForce::IOC,
            .quantity = quant::core::Quantity{10},
            .price = tick.ask_price};
        const auto oid = router_.route_order(gateway_, buy_req, pos.quantity);
        if (oid.value != 0) {
          ++orders_sent_;
        }
      }

      if (pos.quantity.value > -500) {
        const quant::execution::OrderRequest sell_req{
            .instrument_id = tick.instrument_id,
            .side = quant::execution::Side::Sell,
            .type = quant::execution::OrderType::Limit,
            .time_in_force = quant::execution::TimeInForce::IOC,
            .quantity = quant::core::Quantity{10},
            .price = tick.bid_price};
        const auto oid = router_.route_order(gateway_, sell_req, pos.quantity);
        if (oid.value != 0) {
          ++orders_sent_;
        }
      }
    }
  }

  void on_trade(const quant::market::Trade & /*trade*/) noexcept override {}
  void on_order_state(const quant::execution::OrderState & /*state*/) noexcept override {}

  void on_fill(const quant::execution::Fill &fill) noexcept override {
    ++fills_received_;
    total_traded_volume_ += fill.quantity.value;
  }

  [[nodiscard]] std::uint64_t ticks_processed() const noexcept { return ticks_processed_; }
  [[nodiscard]] std::uint64_t orders_sent() const noexcept { return orders_sent_; }
  [[nodiscard]] std::uint64_t fills_received() const noexcept { return fills_received_; }
  [[nodiscard]] std::int64_t total_traded_volume() const noexcept { return total_traded_volume_; }

private:
  quant::core::StrategyId id_;
  quant::execution::OrderRouter &router_;
  quant::execution::SimulatorGateway &gateway_;
  quant::portfolio::PortfolioManager &pm_;

  std::uint64_t ticks_processed_{0};
  std::uint64_t orders_sent_{0};
  std::uint64_t fills_received_{0};
  std::int64_t total_traded_volume_{0};
};

/// Composite execution listener forwarding fills to OrderManager, PortfolioManager, and StrategyEngine
class EngineExecutionListener : public quant::execution::IExecutionListener {
public:
  EngineExecutionListener(quant::execution::OrderManager &om,
                          quant::portfolio::PortfolioManager &pm,
                          quant::strategy::StrategyEngine &se)
      : om_(om), pm_(pm), se_(se) {}

  void on_fill(const quant::execution::Fill &fill) noexcept override {
    om_.on_fill(fill);
    se_.on_fill(fill);
  }

  void on_execution_report(const quant::execution::ExecutionReport &report) noexcept override {
    om_.on_execution_report(report);
    if (report.last_quantity.value > 0) {
      const quant::execution::Fill fill{
          .order_id = report.order_id,
          .execution_id = report.execution_id,
          .timestamp = report.timestamp,
          .price = report.last_price,
          .quantity = report.last_quantity};
      pm_.on_fill(quant::core::InstrumentId{101}, report.side, fill);
    }
  }

private:
  quant::execution::OrderManager &om_;
  quant::portfolio::PortfolioManager &pm_;
  quant::strategy::StrategyEngine &se_;
};

/// Feed subscriber adapter to forward ticks to strategy engine, portfolio, and matching simulator
class EngineTickSubscriber : public quant::market::IMarketDataSubscriber {
public:
  EngineTickSubscriber(quant::strategy::StrategyEngine &se,
                       quant::portfolio::PortfolioManager &pm,
                       quant::execution::Simulator &sim)
      : se_(se), pm_(pm), sim_(sim) {}

  void on_market_tick(const quant::market::MarketTick &tick) noexcept override {
    pm_.on_market_tick(tick);
    se_.on_market_tick(tick);
    sim_.on_market_tick(tick);
  }

  void on_trade(const quant::market::Trade &trade) noexcept override {
    se_.on_trade(trade);
  }

  void on_order_book(const quant::market::Level2Book & /*book*/) noexcept override {}

private:
  quant::strategy::StrategyEngine &se_;
  quant::portfolio::PortfolioManager &pm_;
  quant::execution::Simulator &sim_;
};

} // namespace

int main() {
  std::cout << "=================================================================\n";
  std::cout << "  QUANTA ULTRA-LOW-LATENCY QUANT TRADING ENGINE (v"
            << quant::core::version_major() << ".1.0)\n";
  std::cout << "=================================================================\n\n";

  constexpr std::size_t NUM_TICKS = 1'000'000;
  std::cout << ">> Initializing simulation for " << NUM_TICKS << " ticks...\n";

  // Preallocate market data ticks
  std::vector<quant::market::MarketTick> ticks;
  ticks.reserve(NUM_TICKS);

  std::int64_t mid_price = 10000;
  for (std::size_t i = 0; i < NUM_TICKS; ++i) {
    if ((i & 0x3) == 0) {
      mid_price += ((i % 5) - 2);
    }
    ticks.push_back(quant::market::MarketTick{
        .instrument_id = quant::core::InstrumentId{101},
        .timestamp = quant::core::Timestamp{quant::core::Duration{static_cast<std::int64_t>(i * 1000)}},
        .bid_price = quant::core::Price{mid_price - 1},
        .bid_quantity = quant::core::Quantity{100},
        .ask_price = quant::core::Price{mid_price + 1},
        .ask_quantity = quant::core::Quantity{100}});
  }

  // Pre-allocate and initialize engine components
  quant::risk::RiskLimits limits{
      .max_order_quantity = quant::core::Quantity{100},
      .max_order_notional = quant::core::Price{10'000'000},
      .max_position_quantity = quant::core::Quantity{1000},
      .max_orders_per_second = 100000,
      .price_collar_bps = 500};

  quant::risk::RiskManager risk_manager(limits);
  quant::execution::OrderManager order_manager;
  order_manager.reserve(NUM_TICKS / 4);

  quant::execution::Simulator simulator;
  simulator.reserve(NUM_TICKS / 4);

  quant::execution::SimulatorGateway gateway(&simulator);

  quant::execution::OrderRouter order_router(risk_manager, order_manager);
  quant::portfolio::PortfolioManager portfolio_manager(quant::core::Price{10'000'000});
  portfolio_manager.reserve(16);

  quant::strategy::StrategyEngine strategy_engine;
  strategy_engine.reserve(4);

  EngineExecutionListener execution_listener(order_manager, portfolio_manager, strategy_engine);
  simulator.set_listener(&execution_listener);

  BenchmarkMarketMakerStrategy strategy(quant::core::StrategyId{1}, order_router, gateway, portfolio_manager);
  strategy_engine.register_strategy(&strategy);

  EngineTickSubscriber tick_subscriber(strategy_engine, portfolio_manager, simulator);
  quant::market::MarketDataDispatcher dispatcher;
  dispatcher.reserve(4);
  dispatcher.register_subscriber(&tick_subscriber);

  quant::market::ReplayFeed feed;
  feed.load_ticks(std::move(ticks));
  feed.start();

  std::cout << ">> Running full critical path: Feed -> Dispatcher -> Strategy -> Risk -> Router -> Matching -> Portfolio...\n\n";

  const auto start_time = std::chrono::steady_clock::now();

  quant::market::MarketTick tick{};
  while (feed.next_tick(tick)) {
    dispatcher.dispatch(tick);
  }

  const auto end_time = std::chrono::steady_clock::now();
  const auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
  const double elapsed_ms = static_cast<double>(elapsed_ns) / 1'000'000.0;
  const double ns_per_tick = static_cast<double>(elapsed_ns) / static_cast<double>(NUM_TICKS);
  const double ticks_per_sec = (static_cast<double>(NUM_TICKS) / static_cast<double>(elapsed_ns)) * 1'000'000'000.0;
  const double million_ticks_per_sec = ticks_per_sec / 1'000'000.0;

  std::cout << "-----------------------------------------------------------------\n";
  std::cout << "  BENCHMARK RESULTS (1,000,000 FULL ENGINE CYCLES)\n";
  std::cout << "-----------------------------------------------------------------\n";
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "  Total Elapsed Time     : " << elapsed_ms << " ms\n";
  std::cout << "  Average Latency / Tick : " << ns_per_tick << " ns\n";
  std::cout << "  Throughput             : " << million_ticks_per_sec << " Million ticks/sec ("
            << static_cast<std::uint64_t>(ticks_per_sec) << " ticks/sec)\n";
  std::cout << "  Orders Submitted       : " << strategy.orders_sent() << "\n";
  std::cout << "  Fills Executed         : " << strategy.fills_received() << "\n";
  std::cout << "  Total Traded Volume    : " << strategy.total_traded_volume() << " units\n";
  std::cout << "  Active Orders Remaining: " << order_manager.active_order_count() << "\n";
  std::cout << "  Portfolio Final Equity : $" << portfolio_manager.portfolio().total_equity.value << "\n";
  std::cout << "  Portfolio Realized PnL : $" << portfolio_manager.portfolio().realized_pnl.value << "\n";
  std::cout << "  Portfolio Unrealized   : $" << portfolio_manager.portfolio().unrealized_pnl.value << "\n";
  std::cout << "-----------------------------------------------------------------\n";

  if (elapsed_ms < 1000.0) {
    std::cout << ">> [SUCCESS] Completed 1,000,000 critical execution cycles in " << elapsed_ms << " ms!\n\n";
  }

  return 0;
}