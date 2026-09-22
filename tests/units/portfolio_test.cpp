#include <cassert>

#include "quant/portfolio/portfolio_manager.hpp"

int main() {
  using namespace quant::portfolio;
  using namespace quant::execution;

  PortfolioManager pm(quant::core::Price{1'000'000});
  assert(pm.position_count() == 0);
  assert(pm.portfolio().cash_balance.value == 1'000'000);

  Fill fill{
      .order_id = {1},
      .execution_id = {1},
      .timestamp = {},
      .price = {150},
      .quantity = {100}};

  pm.on_fill(quant::core::InstrumentId{101}, Side::Buy, fill);
  assert(pm.position_count() == 1);

  Position pos{};
  assert(pm.get_position(quant::core::InstrumentId{101}, pos));
  assert(pos.quantity.value == 100);
  assert(pos.average_entry_price.value == 150);
  assert(pos.is_long());

  return 0;
}
