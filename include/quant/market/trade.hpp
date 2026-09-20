#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/time.hpp"

namespace quant::market {

struct Trade {
  core::InstrumentId instrument_id{};
  core::Timestamp timestamp{};
  core::Price price{};
  core::Quantity quantity{};
};

} // namespace quant::market