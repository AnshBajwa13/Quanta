#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/execution/order_status.hpp"

namespace quant::execution {

struct OrderState {
  core::OrderId order_id{};
  OrderStatus status{};

  core::Quantity requested_quantity{};
  core::Quantity filled_quantity{};

  core::Price average_fill_price{};
};

} // namespace quant::execution