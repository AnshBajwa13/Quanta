#pragma once

#include "quant/execution/fill.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/market/market_tick.hpp"

namespace quant::execution {

class MatchingEngine {
public:
  static bool match(const OrderRequest &order, const market::MarketTick &tick,
                    core::OrderId order_id, core::ExecutionId execution_id,
                    Fill &fill) noexcept;
};

} // namespace quant::execution