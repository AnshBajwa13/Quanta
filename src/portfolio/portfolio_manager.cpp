#include "quant/portfolio/portfolio_manager.hpp"

#include <cmath>

namespace quant::portfolio {

PortfolioManager::PortfolioManager(core::Price initial_cash) {
  portfolio_.cash_balance = initial_cash;
  portfolio_.total_equity = initial_cash;
  portfolio_.margin_available = initial_cash;
}

void PortfolioManager::reserve(std::size_t capacity) {
  positions_.reserve(capacity);
  pos_index_.reserve(capacity);
}

void PortfolioManager::on_fill(core::InstrumentId instrument_id, execution::Side side,
                               const execution::Fill &fill) noexcept {
  const auto it = pos_index_.find(instrument_id.value);
  if (it != pos_index_.end()) {
    auto &pos = positions_[it->second];
    const std::int64_t fill_qty = (side == execution::Side::Buy) ? fill.quantity.value : -fill.quantity.value;
    const std::int64_t old_qty = pos.quantity.value;
    const std::int64_t new_qty = old_qty + fill_qty;

    if (old_qty == 0) {
      pos.quantity = core::Quantity{new_qty};
      pos.average_entry_price = fill.price;
    } else if ((old_qty > 0 && fill_qty > 0) || (old_qty < 0 && fill_qty < 0)) {
      // Adding to existing position in the same direction: weighted average price
      const std::int64_t abs_old = std::abs(old_qty);
      const std::int64_t abs_fill = std::abs(fill_qty);
      const std::int64_t abs_new = abs_old + abs_fill;
      if (abs_new > 0) {
        const std::int64_t total_val = abs_old * pos.average_entry_price.value + abs_fill * fill.price.value;
        pos.average_entry_price = core::Price{total_val / abs_new};
      }
      pos.quantity = core::Quantity{new_qty};
    } else {
      // Reducing or flipping position
      const std::int64_t closed_qty = std::min(std::abs(old_qty), std::abs(fill_qty));
      std::int64_t realized = 0;
      if (old_qty > 0) {
        realized = closed_qty * (fill.price.value - pos.average_entry_price.value);
      } else {
        realized = closed_qty * (pos.average_entry_price.value - fill.price.value);
      }
      pos.realized_pnl.value += realized;
      portfolio_.realized_pnl.value += realized;

      if ((old_qty > 0 && new_qty < 0) || (old_qty < 0 && new_qty > 0)) {
        // Position flipped: remaining portion has entry price = fill price
        pos.average_entry_price = fill.price;
      }
      pos.quantity = core::Quantity{new_qty};
    }
    return;
  }

  // New position
  const std::size_t index = positions_.size();
  Position new_pos{
      .instrument_id = instrument_id,
      .quantity = core::Quantity{side == execution::Side::Buy ? fill.quantity.value : -fill.quantity.value},
      .average_entry_price = fill.price,
      .realized_pnl = core::Price{0},
      .unrealized_pnl = core::Price{0}};
  positions_.push_back(new_pos);
  pos_index_[instrument_id.value] = index;
}

void PortfolioManager::on_market_tick(const market::MarketTick &tick) noexcept {
  const auto it = pos_index_.find(tick.instrument_id.value);
  if (it != pos_index_.end()) {
    auto &pos = positions_[it->second];
    if (pos.quantity.value > 0) {
      const std::int64_t current_price = (tick.bid_price.value > 0) ? tick.bid_price.value : tick.ask_price.value;
      if (current_price > 0) {
        pos.unrealized_pnl = core::Price{pos.quantity.value * (current_price - pos.average_entry_price.value)};
      }
    } else if (pos.quantity.value < 0) {
      const std::int64_t current_price = (tick.ask_price.value > 0) ? tick.ask_price.value : tick.bid_price.value;
      if (current_price > 0) {
        pos.unrealized_pnl = core::Price{(-pos.quantity.value) * (pos.average_entry_price.value - current_price)};
      }
    } else {
      pos.unrealized_pnl = core::Price{0};
    }
  }

  std::int64_t total_unrealized = 0;
  for (const auto &pos : positions_) {
    total_unrealized += pos.unrealized_pnl.value;
  }
  portfolio_.unrealized_pnl = core::Price{total_unrealized};
  portfolio_.total_equity = core::Price{portfolio_.cash_balance.value + portfolio_.realized_pnl.value + total_unrealized};
}

bool PortfolioManager::get_position(core::InstrumentId instrument_id,
                                    Position &position) const noexcept {
  const auto it = pos_index_.find(instrument_id.value);
  if (it != pos_index_.end()) {
    position = positions_[it->second];
    return true;
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
