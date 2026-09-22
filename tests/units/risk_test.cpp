#include <cassert>

#include "quant/risk/kill_switch.hpp"
#include "quant/risk/risk_manager.hpp"

int main() {
  using namespace quant::risk;
  using namespace quant::execution;

  RiskLimits limits{
      .max_order_quantity = {100},
      .max_order_notional = {100000},
      .max_position_quantity = {500},
      .max_orders_per_second = 50,
      .price_collar_bps = 500};

  RiskManager risk_manager(limits);
  assert(!risk_manager.kill_switch().is_tripped());

  OrderRequest valid_req{
      .instrument_id = {1},
      .side = Side::Buy,
      .type = OrderType::Limit,
      .time_in_force = TimeInForce::Day,
      .quantity = {50},
      .price = {100}};

  auto res = risk_manager.check_order(valid_req, quant::core::Quantity{0});
  assert(res.is_approved());

  // Test exceeding max order quantity
  OrderRequest huge_req = valid_req;
  huge_req.quantity = {200};
  res = risk_manager.check_order(huge_req, quant::core::Quantity{0});
  assert(!res.is_approved());
  assert(res.reason == RiskRejectReason::ExceedsMaxOrderQuantity);

  // Test kill switch
  risk_manager.kill_switch().trip(RiskRejectReason::KillSwitchActive, "Manual halt");
  assert(risk_manager.kill_switch().is_tripped());
  res = risk_manager.check_order(valid_req, quant::core::Quantity{0});
  assert(!res.is_approved());
  assert(res.reason == RiskRejectReason::KillSwitchActive);

  risk_manager.kill_switch().reset();
  assert(!risk_manager.kill_switch().is_tripped());
  res = risk_manager.check_order(valid_req, quant::core::Quantity{0});
  assert(res.is_approved());

  return 0;
}
