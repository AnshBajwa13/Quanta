#include "quant/risk/kill_switch.hpp"

namespace quant::risk {

void KillSwitch::trip(RiskRejectReason reason, std::string_view message) noexcept {
  reason_.store(reason, std::memory_order_relaxed);
  message_ = message;
  tripped_.store(true, std::memory_order_release);
}

void KillSwitch::reset() noexcept {
  reason_.store(RiskRejectReason::None, std::memory_order_relaxed);
  message_ = {};
  tripped_.store(false, std::memory_order_release);
}

bool KillSwitch::is_tripped() const noexcept {
  return tripped_.load(std::memory_order_acquire);
}

RiskRejectReason KillSwitch::reason() const noexcept {
  return reason_.load(std::memory_order_relaxed);
}

std::string_view KillSwitch::message() const noexcept {
  return message_;
}

} // namespace quant::risk
