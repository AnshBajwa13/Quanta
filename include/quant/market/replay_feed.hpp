#pragma once

#include <cstddef>
#include <vector>

#include "quant/core/ids.hpp"
#include "quant/core/result.hpp"
#include "quant/market/market_data_feed.hpp"
#include "quant/market/market_tick.hpp"

namespace quant::market {

class ReplayFeed {
public:
  ReplayFeed() = default;
  ~ReplayFeed() = default;

  void load_ticks(std::vector<MarketTick> ticks);

  core::Status subscribe(core::InstrumentId instrument_id);
  core::Status unsubscribe(core::InstrumentId instrument_id);

  core::Status start();
  core::Status stop();

  [[nodiscard]] bool is_connected() const noexcept;
  [[nodiscard]] bool has_next() const noexcept;
  [[nodiscard]] bool next_tick(MarketTick &tick) noexcept;
  [[nodiscard]] std::size_t remaining_ticks() const noexcept;

private:
  std::vector<MarketTick> ticks_;
  std::size_t cursor_{0};
  bool running_{false};
};

static_assert(MarketDataFeed<ReplayFeed>);

} // namespace quant::market
