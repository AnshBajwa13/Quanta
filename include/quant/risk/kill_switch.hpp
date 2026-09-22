#pragma once

#include <atomic>
#include <string_view>

#include "quant/risk/risk_result.hpp"

namespace quant::risk {

class KillSwitch {
public:
  KillSwitch() = default;
  ~KillSwitch() = default;

  KillSwitch(const KillSwitch &) = delete;
  KillSwitch &operator=(const KillSwitch &) = delete;

  void trip(RiskRejectReason reason = RiskRejectReason::KillSwitchActive,
            std::string_view message = "Kill switch activated") noexcept;

  void reset() noexcept;

  [[nodiscard]] bool is_tripped() const noexcept;
  [[nodiscard]] RiskRejectReason reason() const noexcept;
  [[nodiscard]] std::string_view message() const noexcept;

private:
  std::atomic<bool> tripped_{false};
  std::atomic<RiskRejectReason> reason_{RiskRejectReason::None};
  std::string_view message_{};
};

} // namespace quant::risk
