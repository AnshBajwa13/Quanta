#pragma once

#include <cstdint>
#include <string_view>

namespace quant::risk {

enum class RiskDecision : std::uint8_t {
  Approved,
  Rejected
};

enum class RiskRejectReason : std::uint16_t {
  None = 0,
  KillSwitchActive,
  ExceedsMaxOrderQuantity,
  ExceedsMaxOrderNotional,
  ExceedsMaxPosition,
  ExceedsRateLimit,
  PriceCollarBreach,
  InvalidInstrument,
  InvalidQuantity,
  InvalidPrice,
  TradingHalted
};

struct RiskCheckResult {
  RiskDecision decision{RiskDecision::Approved};
  RiskRejectReason reason{RiskRejectReason::None};
  std::string_view message{};

  [[nodiscard]] constexpr bool is_approved() const noexcept {
    return decision == RiskDecision::Approved;
  }

  [[nodiscard]] constexpr explicit operator bool() const noexcept {
    return is_approved();
  }
};

} // namespace quant::risk
