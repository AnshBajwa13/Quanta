#pragma once

#include <cstddef>
#include <vector>

#include "quant/core/ids.hpp"
#include "quant/core/price.hpp"
#include "quant/core/quantity.hpp"
#include "quant/execution/fill.hpp"
#include "quant/execution/side.hpp"
#include "quant/market/market_tick.hpp"
#include "quant/portfolio/portfolio.hpp"
#include "quant/portfolio/position.hpp"

namespace quant::portfolio {

class PortfolioManager {
public:
  explicit PortfolioManager(core::Price initial_cash = core::Price{0});
  ~PortfolioManager() = default;

  void on_fill(core::InstrumentId instrument_id, execution::Side side,
               const execution::Fill &fill) noexcept;

  void on_market_tick(const market::MarketTick &tick) noexcept;

  [[nodiscard]] bool get_position(core::InstrumentId instrument_id,
                                  Position &position) const noexcept;

  [[nodiscard]] const Portfolio &portfolio() const noexcept;
  [[nodiscard]] std::size_t position_count() const noexcept;

private:
  Portfolio portfolio_{};
  std::vector<Position> positions_;
};

} // namespace quant::portfolio
