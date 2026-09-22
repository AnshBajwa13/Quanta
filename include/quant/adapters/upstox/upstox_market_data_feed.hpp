#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/result.hpp"
#include "quant/market/market_data_dispatcher.hpp"
#include "quant/market/market_data_feed.hpp"

namespace quant::adapters::upstox {

class UpstoxMarketDataFeed {
public:
  UpstoxMarketDataFeed() = default;
  ~UpstoxMarketDataFeed() = default;

  core::Status subscribe(core::InstrumentId instrument_id);
  core::Status unsubscribe(core::InstrumentId instrument_id);

  core::Status start();
  core::Status stop();

  [[nodiscard]] bool is_connected() const noexcept;

  void set_subscriber(market::IMarketDataSubscriber *subscriber) noexcept;

private:
  market::IMarketDataSubscriber *subscriber_{nullptr};
  bool connected_{false};
};

static_assert(market::MarketDataFeed<UpstoxMarketDataFeed>);

} // namespace quant::adapters::upstox
