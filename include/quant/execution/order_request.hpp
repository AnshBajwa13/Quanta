#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/execution/order_type.hpp"
#include "quant/execution/side.hpp"
#include "quant/execution/time_in_force.hpp"

namespace quant::execution {

struct OrderRequest {
  core::InstrumentId instrument_id{};
  Side side{};
  OrderType type{};
  TimeInForce time_in_force{};

  core::Quantity quantity{};
  core::Price price{};
};

} // namespace quant::execution