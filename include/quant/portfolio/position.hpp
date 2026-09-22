#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"

namespace quant::portfolio {

struct Position {
  core::InstrumentId instrument_id{};
  core::Quantity quantity{}; // Positive = Long, Negative = Short, 0 = Flat
  core::Price average_entry_price{};
  core::Price realized_pnl{};
  core::Price unrealized_pnl{};

  [[nodiscard]] constexpr bool is_flat() const noexcept {
    return quantity.value == 0;
  }

  [[nodiscard]] constexpr bool is_long() const noexcept {
    return quantity.value > 0;
  }

  [[nodiscard]] constexpr bool is_short() const noexcept {
    return quantity.value < 0;
  }
};

} // namespace quant::portfolio
