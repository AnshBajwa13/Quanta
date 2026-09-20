#include <cassert>
#include <type_traits>

#include "quant/execution/execution_gateway.hpp"

namespace {

class TestGateway {
public:
  quant::core::OrderId submit(const quant::execution::OrderRequest &) {
    return quant::core::OrderId{42};
  }

  void cancel(quant::core::OrderId) {}
};

class InvalidGateway {
public:
  int submit(const quant::execution::OrderRequest &) { return 42; }
};

} // namespace

int main() {
  using namespace quant::execution;

  static_assert(ExecutionGateway<TestGateway>);
  static_assert(!ExecutionGateway<InvalidGateway>);

  TestGateway gateway;

  OrderRequest request{.instrument_id = quant::core::InstrumentId{1},
                       .side = Side::Buy,
                       .type = OrderType::Limit,
                       .time_in_force = TimeInForce::Day,
                       .quantity = quant::core::Quantity{100},
                       .price = quant::core::Price{10005}};

  const auto order_id = gateway.submit(request);

  assert(order_id.value == 42);

  gateway.cancel(order_id);

  return 0;
}