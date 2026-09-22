#pragma once

#include <algorithm>

#include "quant/execution/fill.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/market/market_tick.hpp"

namespace quant::execution {

class MatchingEngine {
public:
  [[nodiscard]] static inline bool match(const OrderRequest &order,
                                         const market::MarketTick &tick,
                                         core::OrderId order_id,
                                         core::ExecutionId execution_id,
                                         Fill &fill) noexcept {
    if (order.type != OrderType::Limit) [[unlikely]] {
      return false;
    }

    core::Price execution_price{};
    core::Quantity available_quantity{};

    if (order.side == Side::Buy) {
      if (order.price.value < tick.ask_price.value) {
        return false;
      }
      execution_price = tick.ask_price;
      available_quantity = tick.ask_quantity;
    } else if (order.side == Side::Sell) {
      if (order.price.value > tick.bid_price.value) {
        return false;
      }
      execution_price = tick.bid_price;
      available_quantity = tick.bid_quantity;
    } else [[unlikely]] {
      return false;
    }

    if (order.time_in_force == TimeInForce::FOK &&
        available_quantity.value < order.quantity.value) [[unlikely]] {
      return false;
    }

    const auto fill_quantity =
        std::min(order.quantity.value, available_quantity.value);

    if (fill_quantity <= 0) [[unlikely]] {
      return false;
    }

    fill = Fill{.order_id = order_id,
                .execution_id = execution_id,
                .timestamp = tick.timestamp,
                .price = execution_price,
                .quantity = core::Quantity{fill_quantity}};

    return true;
  }
};

} // namespace quant::execution