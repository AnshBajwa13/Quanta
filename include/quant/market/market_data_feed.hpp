#pragma once

#include <concepts>

#include "quant/core/ids.hpp"
#include "quant/core/result.hpp"

namespace quant::market {

template <typename T>
concept MarketDataFeed =
    requires(T &feed, const T &cfeed, core::InstrumentId instrument_id) {
      { feed.subscribe(instrument_id) } -> std::same_as<core::Status>;
      { feed.unsubscribe(instrument_id) } -> std::same_as<core::Status>;
      { feed.start() } -> std::same_as<core::Status>;
      { feed.stop() } -> std::same_as<core::Status>;
      { cfeed.is_connected() } noexcept -> std::same_as<bool>;
    };

} // namespace quant::market
