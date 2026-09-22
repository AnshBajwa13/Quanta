#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/quantity.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/risk/kill_switch.hpp"
#include "quant/risk/risk_limits.hpp"
#include "quant/risk/risk_result.hpp"

namespace quant::risk {

class RiskManager {
public:
  explicit RiskManager(RiskLimits limits = {});
  ~RiskManager() = default;

  [[nodiscard]] RiskCheckResult check_order(const execution::OrderRequest &request,
                                           core::Quantity current_position) const noexcept;

  [[nodiscard]] RiskCheckResult check_cancellation(core::OrderId order_id) const noexcept;

  void set_limits(const RiskLimits &limits) noexcept;
  [[nodiscard]] const RiskLimits &limits() const noexcept;

  [[nodiscard]] KillSwitch &kill_switch() noexcept;
  [[nodiscard]] const KillSwitch &kill_switch() const noexcept;

private:
  RiskLimits limits_{};
  KillSwitch kill_switch_{};
};

} // namespace quant::risk
