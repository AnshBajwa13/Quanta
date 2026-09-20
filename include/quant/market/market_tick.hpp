#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/time.hpp"

namespace quant::market {

struct MarketTick {
  core::InstrumentId instrument_id{};
  core::Timestamp timestamp{};

  core::Price bid_price{};
  core::Quantity bid_quantity{};

  core::Price ask_price{};
  core::Quantity ask_quantity{};
};

} // namespace quant::market