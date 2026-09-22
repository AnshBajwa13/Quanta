#include "quant/portfolio/portfolio_manager.hpp"

namespace quant::portfolio {

PortfolioManager::PortfolioManager(core::Price initial_cash) {
  portfolio_.cash_balance = initial_cash;
  portfolio_.total_equity = initial_cash;
  portfolio_.margin_available = initial_cash;
}

void PortfolioManager::on_fill(core::InstrumentId instrument_id, execution::Side side,
                               const execution::Fill &fill) noexcept {
  for (auto &pos : positions_) {
    if (pos.instrument_id.value == instrument_id.value) {
      if (side == execution::Side::Buy) {
        pos.quantity.value += fill.quantity.value;
      } else {
        pos.quantity.value -= fill.quantity.value;
      }
      return;
    }
  }

  // New position
  Position new_pos{
      .instrument_id = instrument_id,
      .quantity = core::Quantity{side == execution::Side::Buy ? fill.quantity.value : -fill.quantity.value},
      .average_entry_price = fill.price,
      .realized_pnl = core::Price{0},
      .unrealized_pnl = core::Price{0}};
  positions_.push_back(new_pos);
}

void PortfolioManager::on_market_tick(const market::MarketTick & /*tick*/) noexcept {
  // SKELETON: Mark-to-market calculations to be implemented
}

bool PortfolioManager::get_position(core::InstrumentId instrument_id,
                                    Position &position) const noexcept {
  for (const auto &pos : positions_) {
    if (pos.instrument_id.value == instrument_id.value) {
      position = pos;
      return true;
    }
  }
  return false;
}

const Portfolio &PortfolioManager::portfolio() const noexcept {
  return portfolio_;
}

std::size_t PortfolioManager::position_count() const noexcept {
  return positions_.size();
}

} // namespace quant::portfolio
