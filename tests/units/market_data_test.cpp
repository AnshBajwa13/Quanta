#include <cassert>

#include "quant/market/market_data_dispatcher.hpp"

class MockSubscriber : public quant::market::IMarketDataSubscriber {
public:
  void on_market_tick(const quant::market::MarketTick &) noexcept override {
    tick_count++;
  }
  void on_trade(const quant::market::Trade &) noexcept override {
    trade_count++;
  }
  void on_order_book(const quant::market::Level2Book &) noexcept override {
    book_count++;
  }

  int tick_count{0};
  int trade_count{0};
  int book_count{0};
};

int main() {
  using namespace quant::market;

  MarketDataDispatcher dispatcher;
  MockSubscriber sub;

  assert(dispatcher.subscriber_count() == 0);
  dispatcher.register_subscriber(&sub);
  assert(dispatcher.subscriber_count() == 1);

  MarketTick tick{.instrument_id = {101}};
  dispatcher.dispatch(tick);
  assert(sub.tick_count == 1);

  Trade trade{.instrument_id = {101}};
  dispatcher.dispatch(trade);
  assert(sub.trade_count == 1);

  Level2Book book{.instrument_id = {101}};
  dispatcher.dispatch(book);
  assert(sub.book_count == 1);

  dispatcher.unregister_subscriber(&sub);
  assert(dispatcher.subscriber_count() == 0);

  return 0;
}
