#pragma once

#include "quant/core/price.hpp"

namespace quant::portfolio {

struct Portfolio {
  core::Price cash_balance{};
  core::Price realized_pnl{};
  core::Price unrealized_pnl{};
  core::Price total_equity{};
  core::Price margin_used{};
  core::Price margin_available{};
};

} // namespace quant::portfolio
