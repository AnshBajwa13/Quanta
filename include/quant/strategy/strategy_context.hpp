#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/time.hpp"
#include "quant/execution/order_request.hpp"

namespace quant::strategy {

/// Context interface provided to strategies during execution.
/// Encapsulates order generation capabilities while enforcing boundary rules.
class IStrategyContext {
public:
  virtual ~IStrategyContext() = default;

  [[nodiscard]] virtual core::OrderId submit_order(core::StrategyId strategy_id,
                                                   const execution::OrderRequest &request) = 0;
  virtual void cancel_order(core::OrderId order_id) = 0;
  [[nodiscard]] virtual core::Timestamp current_time() const noexcept = 0;
};

} // namespace quant::strategy
