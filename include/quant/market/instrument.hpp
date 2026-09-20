#pragma once

#include <cstdint>
#include <string_view>

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"

namespace quant::market {

struct Instrument {
  core::InstrumentId id{};
  std::string_view symbol{};
  core::Price tick_size{};
  core::Quantity lot_size{};
};

} // namespace quant::market