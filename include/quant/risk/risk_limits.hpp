#pragma once

#include <cstdint>

#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"

namespace quant::risk {

struct RiskLimits {
  core::Quantity max_order_quantity{10000};
  core::Price max_order_notional{100'000'000};
  core::Quantity max_position_quantity{50000};
  std::uint32_t max_orders_per_second{100};
  std::uint32_t price_collar_bps{500}; // 5% collar
};

} // namespace quant::risk
