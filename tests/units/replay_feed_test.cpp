#include <cassert>
#include <vector>

#include "quant/market/replay_feed.hpp"

int main() {
  using namespace quant::market;
  using namespace quant::core;

  ReplayFeed feed;
  assert(!feed.is_connected());

  std::vector<MarketTick> sample_ticks;
  sample_ticks.push_back(MarketTick{.instrument_id = {101}, .bid_price = {Price{100}}});
  sample_ticks.push_back(MarketTick{.instrument_id = {101}, .bid_price = {Price{105}}});

  feed.load_ticks(sample_ticks);
  assert(feed.remaining_ticks() == 2);

  feed.start();
  assert(feed.is_connected());
  assert(feed.has_next());

  MarketTick t1;
  assert(feed.next_tick(t1));
  assert(t1.bid_price.value == 100);
  assert(feed.remaining_ticks() == 1);

  MarketTick t2;
  assert(feed.next_tick(t2));
  assert(t2.bid_price.value == 105);
  assert(feed.remaining_ticks() == 0);
  assert(!feed.has_next());

  feed.stop();
  assert(!feed.is_connected());

  return 0;
}
