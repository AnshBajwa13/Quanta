#pragma once

#include <compare>
#include <cstdint>

namespace quant::core {

struct Price {
  std::int64_t value{};

  constexpr auto operator<=>(const Price &) const = default;
};

} // namespace quant::core