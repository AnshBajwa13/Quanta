#pragma once

#include <cstdint>
#include <string_view>

namespace quant::core {

enum class StatusCode : std::uint16_t {
  Success = 0,
  GeneralError,
  InvalidArgument,
  BufferFull,
  BufferEmpty,
  NotFound,
  AlreadyExists,
  RiskCheckFailed,
  ConnectionFailed,
  Timeout,
  Disconnected
};

struct Status {
  StatusCode code{StatusCode::Success};
  std::string_view message{};

  [[nodiscard]] constexpr bool is_ok() const noexcept {
    return code == StatusCode::Success;
  }

  [[nodiscard]] constexpr explicit operator bool() const noexcept {
    return is_ok();
  }
};

} // namespace quant::core
