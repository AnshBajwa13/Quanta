#pragma once

#include <chrono>

namespace quant::core {

using Duration = std::chrono::nanoseconds;

struct Timestamp {

  Duration value{};

  constexpr auto operator<=>(const Timestamp &) const = default;
};

} // namespace quant::core