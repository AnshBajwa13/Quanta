#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/time.hpp"

namespace quant::market {

struct PriceLevel {
  core::Price price{};
  core::Quantity quantity{};
  std::uint32_t order_count{};

  constexpr auto operator<=>(const PriceLevel &) const = default;
};

template <std::size_t Depth = 5>
struct OrderBook {
  core::InstrumentId instrument_id{};
  core::Timestamp timestamp{};

  std::array<PriceLevel, Depth> bids{};
  std::array<PriceLevel, Depth> asks{};
  std::uint8_t bid_depth{};
  std::uint8_t ask_depth{};
};

using Level2Book = OrderBook<5>;
using Level3Book = OrderBook<10>;

} // namespace quant::market
