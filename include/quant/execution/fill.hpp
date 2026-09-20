#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/time.hpp"

namespace quant::execution {

struct Fill {
  core::OrderId order_id{};
  core::ExecutionId execution_id{};

  core::Timestamp timestamp{};

  core::Price price{};
  core::Quantity quantity{};
};

} // namespace quant::execution