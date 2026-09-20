#include "quant/execution/matching_engine.hpp"

#include <algorithm>

namespace quant::execution {

bool MatchingEngine::match(const OrderRequest &order,
                           const market::MarketTick &tick,
                           core::OrderId order_id,
                           core::ExecutionId execution_id,
                           Fill &fill) noexcept {
  if (order.type != OrderType::Limit) {
    return false;
  }

  core::Price execution_price{};
  core::Quantity available_quantity{};

  switch (order.side) {
  case Side::Buy:
    if (order.type == OrderType::Limit &&
        order.price.value < tick.ask_price.value) {
      return false;
    }

    execution_price = tick.ask_price;
    available_quantity = tick.ask_quantity;
    break;

  case Side::Sell:
    if (order.type == OrderType::Limit &&
        order.price.value > tick.bid_price.value) {
      return false;
    }

    execution_price = tick.bid_price;
    available_quantity = tick.bid_quantity;
    break;

  default:
    return false;
  }
  if (order.time_in_force == TimeInForce::FOK &&
      available_quantity.value < order.quantity.value) {
    return false;
  }

  const auto fill_quantity =
      std::min(order.quantity.value, available_quantity.value);

  if (fill_quantity <= 0) {
    return false;
  }

  fill = Fill{.order_id = order_id,
              .execution_id = execution_id,
              .timestamp = tick.timestamp,
              .price = execution_price,
              .quantity = core::Quantity{fill_quantity}};

  return true;
}

} // namespace quant::execution