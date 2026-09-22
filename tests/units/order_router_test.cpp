#include <cassert>

#include "quant/execution/order_manager.hpp"
#include "quant/execution/order_router.hpp"
#include "quant/execution/simulator_gateway.hpp"
#include "quant/risk/risk_manager.hpp"

int main() {
  using namespace quant::execution;
  using namespace quant::risk;

  RiskLimits limits{.max_order_quantity = {100}, .max_position_quantity = {500}};
  RiskManager risk_manager(limits);
  OrderManager order_manager;
  OrderRouter router(risk_manager, order_manager);
  SimulatorGateway gateway;

  OrderRequest req{
      .instrument_id = {101},
      .side = Side::Buy,
      .type = OrderType::Limit,
      .time_in_force = TimeInForce::Day,
      .quantity = {50},
      .price = {200}};

  auto order_id = router.route_order(gateway, req, quant::core::Quantity{0});
  assert(order_id.value != 0);
  assert(order_manager.is_active(order_id));

  // Rejected order
  OrderRequest huge_req = req;
  huge_req.quantity = {500};
  auto rejected_id = router.route_order(gateway, huge_req, quant::core::Quantity{0});
  assert(rejected_id.value == 0);

  return 0;
}
