#include "quant/adapters/upstox/upstox_market_data_feed.hpp"

namespace quant::adapters::upstox {

core::Status UpstoxMarketDataFeed::subscribe(core::InstrumentId /*instrument_id*/) {
  // SKELETON: Upstox WebSocket subscription logic to be implemented
  return core::Status{core::StatusCode::Success, {}};
}

core::Status UpstoxMarketDataFeed::unsubscribe(core::InstrumentId /*instrument_id*/) {
  // SKELETON: Upstox WebSocket unsubscription logic to be implemented
  return core::Status{core::StatusCode::Success, {}};
}

core::Status UpstoxMarketDataFeed::start() {
  // SKELETON: Connect to Upstox Market Data Feed WebSocket
  connected_ = true;
  return core::Status{core::StatusCode::Success, {}};
}

core::Status UpstoxMarketDataFeed::stop() {
  // SKELETON: Disconnect from Upstox Market Data Feed WebSocket
  connected_ = false;
  return core::Status{core::StatusCode::Success, {}};
}

bool UpstoxMarketDataFeed::is_connected() const noexcept {
  return connected_;
}

void UpstoxMarketDataFeed::set_subscriber(market::IMarketDataSubscriber *subscriber) noexcept {
  subscriber_ = subscriber;
}

} // namespace quant::adapters::upstox
