#include "quant/market/market_data_dispatcher.hpp"

#include <algorithm>

namespace quant::market {

void MarketDataDispatcher::reserve(std::size_t capacity) {
  subscribers_.reserve(capacity);
}

void MarketDataDispatcher::register_subscriber(IMarketDataSubscriber *subscriber) {
  if (subscriber != nullptr) {
    if (std::find(subscribers_.begin(), subscribers_.end(), subscriber) == subscribers_.end()) {
      subscribers_.push_back(subscriber);
    }
  }
}

void MarketDataDispatcher::unregister_subscriber(IMarketDataSubscriber *subscriber) {
  subscribers_.erase(
      std::remove(subscribers_.begin(), subscribers_.end(), subscriber),
      subscribers_.end());
}

void MarketDataDispatcher::dispatch(const MarketTick &tick) noexcept {
  for (auto *subscriber : subscribers_) {
    subscriber->on_market_tick(tick);
  }
}

void MarketDataDispatcher::dispatch(const Trade &trade) noexcept {
  for (auto *subscriber : subscribers_) {
    subscriber->on_trade(trade);
  }
}

void MarketDataDispatcher::dispatch(const Level2Book &book) noexcept {
  for (auto *subscriber : subscribers_) {
    subscriber->on_order_book(book);
  }
}

std::size_t MarketDataDispatcher::subscriber_count() const noexcept {
  return subscribers_.size();
}

} // namespace quant::market
