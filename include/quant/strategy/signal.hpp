#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/time.hpp"
#include "quant/execution/side.hpp"

namespace quant::strategy {

struct Signal {
  core::StrategyId strategy_id{};
  core::InstrumentId instrument_id{};
  core::Timestamp timestamp{};

  execution::Side side{};
  core::Quantity target_quantity{};
  core::Price limit_price{};

  double strength{1.0};
  double confidence{1.0};
};

} // namespace quant::strategy
