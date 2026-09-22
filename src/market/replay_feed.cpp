#include "quant/market/replay_feed.hpp"

#include <utility>

namespace quant::market {

void ReplayFeed::load_ticks(std::vector<MarketTick> ticks) {
  ticks_ = std::move(ticks);
  cursor_ = 0;
}

core::Status ReplayFeed::subscribe(core::InstrumentId /*instrument_id*/) {
  return core::Status{core::StatusCode::Success, {}};
}

core::Status ReplayFeed::unsubscribe(core::InstrumentId /*instrument_id*/) {
  return core::Status{core::StatusCode::Success, {}};
}

core::Status ReplayFeed::start() {
  running_ = true;
  cursor_ = 0;
  return core::Status{core::StatusCode::Success, {}};
}

core::Status ReplayFeed::stop() {
  running_ = false;
  return core::Status{core::StatusCode::Success, {}};
}

bool ReplayFeed::is_connected() const noexcept {
  return running_;
}

bool ReplayFeed::has_next() const noexcept {
  return cursor_ < ticks_.size();
}

bool ReplayFeed::next_tick(MarketTick &tick) noexcept {
  if (cursor_ < ticks_.size()) {
    tick = ticks_[cursor_++];
    return true;
  }
  return false;
}

std::size_t ReplayFeed::remaining_ticks() const noexcept {
  if (cursor_ >= ticks_.size()) {
    return 0;
  }
  return ticks_.size() - cursor_;
}

} // namespace quant::market
