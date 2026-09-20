#include <cassert>
#include <type_traits>

#include "quant/execution/simulator_gateway.hpp"

int main() {
  using namespace quant::core;
  using namespace quant::execution;

  SimulatorGateway gateway;

  OrderRequest request{.instrument_id = InstrumentId{1},
                       .side = Side::Buy,
                       .type = OrderType::Limit,
                       .time_in_force = TimeInForce::Day,
                       .quantity = Quantity{100},
                       .price = Price{10005}};

  const auto first = gateway.submit(request);
  const auto second = gateway.submit(request);

  assert(first.value == 1);
  assert(second.value == 2);

  gateway.cancel(first);
  gateway.cancel(second);

  static_assert(std::is_empty_v<OrderRequest> == false);

  return 0;
}