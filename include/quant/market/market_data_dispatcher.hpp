#pragma once

#include <cstddef>
#include <tuple>
#include <utility>
#include <vector>

#include "quant/market/market_tick.hpp"
#include "quant/market/order_book.hpp"
#include "quant/market/trade.hpp"

namespace quant::market {

/// Interface for dispatching normalized market events across the engine.
class IMarketDataSubscriber {
public:
  virtual ~IMarketDataSubscriber() = default;

  virtual void on_market_tick(const MarketTick &tick) noexcept = 0;
  virtual void on_trade(const Trade &trade) noexcept = 0;
  virtual void on_order_book(const Level2Book &book) noexcept = 0;
};

class MarketDataDispatcher {
public:
  MarketDataDispatcher() = default;
  ~MarketDataDispatcher() = default;

  void reserve(std::size_t capacity);

  void register_subscriber(IMarketDataSubscriber *subscriber);
  void unregister_subscriber(IMarketDataSubscriber *subscriber);

  void dispatch(const MarketTick &tick) noexcept;
  void dispatch(const Trade &trade) noexcept;
  void dispatch(const Level2Book &book) noexcept;

  [[nodiscard]] std::size_t subscriber_count() const noexcept;

private:
  std::vector<IMarketDataSubscriber *> subscribers_;
};

/// Zero-overhead compile-time static market data dispatcher (no virtual dispatch overhead).
template <typename... Listeners>
class StaticMarketDataDispatcher {
public:
  explicit StaticMarketDataDispatcher(Listeners &...listeners) : listeners_(listeners...) {}

  inline void dispatch(const MarketTick &tick) noexcept {
    std::apply([&tick](auto &...l) { (l.on_market_tick(tick), ...); }, listeners_);
  }

  inline void dispatch(const Trade &trade) noexcept {
    std::apply([&trade](auto &...l) { (l.on_trade(trade), ...); }, listeners_);
  }

  inline void dispatch(const Level2Book &book) noexcept {
    std::apply([&book](auto &...l) { (l.on_order_book(book), ...); }, listeners_);
  }

private:
  std::tuple<Listeners &...> listeners_;
};

} // namespace quant::market
