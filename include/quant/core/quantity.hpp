#pragma once

#include <compare>
#include <cstdint>


namespace quant::core {

struct Quantity {
  std::int64_t value{};

  constexpr auto operator<=>(const Quantity &) const = default;
};

} // namespace quant::core