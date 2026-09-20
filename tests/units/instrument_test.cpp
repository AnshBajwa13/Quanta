#include <cassert>
#include <type_traits>

#include "quant/market/instrument.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::market;

  Instrument instrument{.id = InstrumentId{1},
                        .symbol = "NIFTY",
                        .tick_size = Price{5},
                        .lot_size = Quantity{75}};

  assert(instrument.id.value == 1);
  assert(instrument.symbol == "NIFTY");
  assert(instrument.tick_size.value == 5);
  assert(instrument.lot_size.value == 75);

  static_assert(std::is_trivially_copyable_v<Instrument>);

  return 0;
}