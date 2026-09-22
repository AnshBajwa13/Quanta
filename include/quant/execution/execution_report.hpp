#pragma once

#include <string_view>

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/core/time.hpp"
#include "quant/execution/order_status.hpp"
#include "quant/execution/side.hpp"

namespace quant::execution {

struct ExecutionReport {
  core::OrderId order_id{};
  core::ExecutionId execution_id{};
  core::Timestamp timestamp{};

  OrderStatus status{OrderStatus::New};
  Side side{Side::Buy};

  core::Price last_price{};
  core::Quantity last_quantity{};

  core::Quantity cumulative_quantity{};
  core::Quantity leaves_quantity{};
  core::Price average_price{};

  std::string_view text{};
};

} // namespace quant::execution
