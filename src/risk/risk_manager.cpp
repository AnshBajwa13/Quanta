#include "quant/risk/risk_manager.hpp"

namespace quant::risk {

RiskManager::RiskManager(RiskLimits limits) : limits_(limits) {}

RiskCheckResult RiskManager::check_order(const execution::OrderRequest &request,
                                        core::Quantity current_position) const noexcept {
  if (kill_switch_.is_tripped()) [[unlikely]] {
    return RiskCheckResult{
        .decision = RiskDecision::Rejected,
        .reason = kill_switch_.reason(),
        .message = kill_switch_.message()};
  }

  if (request.quantity.value <= 0) [[unlikely]] {
    return RiskCheckResult{
        .decision = RiskDecision::Rejected,
        .reason = RiskRejectReason::InvalidQuantity,
        .message = "Order quantity must be positive"};
  }

  if (request.price.value <= 0) [[unlikely]] {
    return RiskCheckResult{
        .decision = RiskDecision::Rejected,
        .reason = RiskRejectReason::InvalidPrice,
        .message = "Order price must be positive"};
  }

  if (request.quantity.value > limits_.max_order_quantity.value) [[unlikely]] {
    return RiskCheckResult{
        .decision = RiskDecision::Rejected,
        .reason = RiskRejectReason::ExceedsMaxOrderQuantity,
        .message = "Order quantity exceeds maximum allowed limit"};
  }

  const auto projected_position =
      (request.side == execution::Side::Buy)
          ? (current_position.value + request.quantity.value)
          : (current_position.value - request.quantity.value);

  const auto abs_projected =
      (projected_position < 0) ? -projected_position : projected_position;

  if (abs_projected > limits_.max_position_quantity.value) [[unlikely]] {
    return RiskCheckResult{
        .decision = RiskDecision::Rejected,
        .reason = RiskRejectReason::ExceedsMaxPosition,
        .message = "Projected position exceeds maximum allowed position limit"};
  }

  return RiskCheckResult{
      .decision = RiskDecision::Approved,
      .reason = RiskRejectReason::None,
      .message = "Approved"};
}

RiskCheckResult RiskManager::check_cancellation(core::OrderId /*order_id*/) const noexcept {
  if (kill_switch_.is_tripped()) [[unlikely]] {
    return RiskCheckResult{
        .decision = RiskDecision::Approved,
        .reason = RiskRejectReason::None,
        .message = "Cancellations allowed when kill switch is active"};
  }

  return RiskCheckResult{
      .decision = RiskDecision::Approved,
      .reason = RiskRejectReason::None,
      .message = "Cancellation approved"};
}

void RiskManager::set_limits(const RiskLimits &limits) noexcept {
  limits_ = limits;
}

const RiskLimits &RiskManager::limits() const noexcept {
  return limits_;
}

KillSwitch &RiskManager::kill_switch() noexcept {
  return kill_switch_;
}

const KillSwitch &RiskManager::kill_switch() const noexcept {
  return kill_switch_;
}

} // namespace quant::risk
