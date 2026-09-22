#include <cassert>
#include <type_traits>

#include "quant/market/order_book.hpp"

int main() {
  using namespace quant::market;

  Level2Book book{};
  book.instrument_id = quant::core::InstrumentId{101};
  book.bids[0] = PriceLevel{.price = quant::core::Price{100}, .quantity = quant::core::Quantity{50}, .order_count = 2};
  book.asks[0] = PriceLevel{.price = quant::core::Price{101}, .quantity = quant::core::Quantity{30}, .order_count = 1};
  book.bid_depth = 1;
  book.ask_depth = 1;

  assert(book.instrument_id.value == 101);
  assert(book.bids[0].price.value == 100);
  assert(book.asks[0].price.value == 101);

  static_assert(std::is_trivially_copyable_v<PriceLevel>);
  static_assert(std::is_trivially_copyable_v<Level2Book>);

  return 0;
}
