#include <cassert>

#include "quant/adapters/upstox/upstox_execution_gateway.hpp"
#include "quant/adapters/upstox/upstox_market_data_feed.hpp"

int main() {
  using namespace quant::adapters::upstox;

  UpstoxMarketDataFeed feed;
  assert(!feed.is_connected());
  assert(feed.start().is_ok());
  assert(feed.is_connected());
  assert(feed.subscribe(quant::core::InstrumentId{101}).is_ok());
  assert(feed.stop().is_ok());
  assert(!feed.is_connected());

  UpstoxExecutionGateway gateway;
  quant::execution::OrderRequest req{
      .instrument_id = {101},
      .side = quant::execution::Side::Buy,
      .type = quant::execution::OrderType::Limit,
      .time_in_force = quant::execution::TimeInForce::Day,
      .quantity = {10},
      .price = {1000}};

  auto order_id = gateway.submit(req);
  assert(order_id.value != 0);

  gateway.cancel(order_id);

  return 0;
}
