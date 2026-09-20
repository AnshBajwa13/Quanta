#include <cassert>

#include "quant/execution/simulator.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::execution;
  using namespace quant::market;

  // -------------------------------------------------------------------------
  // DAY order: partial fill followed by completion.
  // -------------------------------------------------------------------------

  {
    Simulator simulator;
    simulator.reserve(16);

    const OrderRequest request{.instrument_id = InstrumentId{1},
                               .side = Side::Buy,
                               .type = OrderType::Limit,
                               .time_in_force = TimeInForce::Day,
                               .quantity = Quantity{100},
                               .price = Price{10100}};

    const OrderId order_id = simulator.submit(request);

    OrderState state{};

    assert(simulator.get_order_state(order_id, state));
    assert(state.order_id.value == order_id.value);
    assert(state.status == OrderStatus::Accepted);
    assert(state.requested_quantity.value == 100);
    assert(state.filled_quantity.value == 0);

    const MarketTick tick{.instrument_id = InstrumentId{1},
                          .timestamp = Timestamp{Duration{1000}},
                          .bid_price = Price{9995},
                          .bid_quantity = Quantity{100},
                          .ask_price = Price{10000},
                          .ask_quantity = Quantity{40}};

    simulator.on_market_tick(tick);

    assert(simulator.get_order_state(order_id, state));
    assert(state.status == OrderStatus::PartiallyFilled);
    assert(state.filled_quantity.value == 40);
    assert(state.average_fill_price.value == 10000);

    const MarketTick tick2{.instrument_id = InstrumentId{1},
                           .timestamp = Timestamp{Duration{2000}},
                           .bid_price = Price{9995},
                           .bid_quantity = Quantity{100},
                           .ask_price = Price{10100},
                           .ask_quantity = Quantity{60}};

    simulator.on_market_tick(tick2);

    assert(simulator.get_order_state(order_id, state));
    assert(state.status == OrderStatus::Filled);
    assert(state.filled_quantity.value == 100);
    assert(state.average_fill_price.value == 10060);
  }

  // -------------------------------------------------------------------------
  // IOC order: partial liquidity fills what is available, then expires.
  // -------------------------------------------------------------------------

  {
    Simulator simulator;
    simulator.reserve(4);

    const OrderRequest request{.instrument_id = InstrumentId{1},
                               .side = Side::Buy,
                               .type = OrderType::Limit,
                               .time_in_force = TimeInForce::IOC,
                               .quantity = Quantity{100},
                               .price = Price{10000}};

    const OrderId order_id = simulator.submit(request);

    const MarketTick tick{.instrument_id = InstrumentId{1},
                          .timestamp = Timestamp{Duration{3000}},
                          .bid_price = Price{9995},
                          .bid_quantity = Quantity{100},
                          .ask_price = Price{10000},
                          .ask_quantity = Quantity{40}};

    simulator.on_market_tick(tick);

    OrderState state{};

    assert(simulator.get_order_state(order_id, state));
    assert(state.status == OrderStatus::Expired);
    assert(state.filled_quantity.value == 40);
    assert(state.average_fill_price.value == 10000);
  }

  // -------------------------------------------------------------------------
  // FOK order: insufficient liquidity means zero fill.
  // -------------------------------------------------------------------------

  {
    Simulator simulator;
    simulator.reserve(4);

    const OrderRequest request{.instrument_id = InstrumentId{1},
                               .side = Side::Buy,
                               .type = OrderType::Limit,
                               .time_in_force = TimeInForce::FOK,
                               .quantity = Quantity{100},
                               .price = Price{10000}};

    const OrderId order_id = simulator.submit(request);

    const MarketTick tick{.instrument_id = InstrumentId{1},
                          .timestamp = Timestamp{Duration{4000}},
                          .bid_price = Price{9995},
                          .bid_quantity = Quantity{100},
                          .ask_price = Price{10000},
                          .ask_quantity = Quantity{40}};

    simulator.on_market_tick(tick);

    OrderState state{};

    assert(simulator.get_order_state(order_id, state));
    assert(state.status == OrderStatus::Expired);
    assert(state.filled_quantity.value == 0);
  }

  // -------------------------------------------------------------------------
  // FOK order: sufficient liquidity means complete fill.
  // -------------------------------------------------------------------------

  {
    Simulator simulator;
    simulator.reserve(4);

    const OrderRequest request{.instrument_id = InstrumentId{1},
                               .side = Side::Buy,
                               .type = OrderType::Limit,
                               .time_in_force = TimeInForce::FOK,
                               .quantity = Quantity{100},
                               .price = Price{10000}};

    const OrderId order_id = simulator.submit(request);

    const MarketTick tick{.instrument_id = InstrumentId{1},
                          .timestamp = Timestamp{Duration{5000}},
                          .bid_price = Price{9995},
                          .bid_quantity = Quantity{100},
                          .ask_price = Price{10000},
                          .ask_quantity = Quantity{100}};

    simulator.on_market_tick(tick);

    OrderState state{};

    assert(simulator.get_order_state(order_id, state));
    assert(state.status == OrderStatus::Filled);
    assert(state.filled_quantity.value == 100);
    assert(state.average_fill_price.value == 10000);
  }

  return 0;
}