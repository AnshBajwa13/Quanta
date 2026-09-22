#pragma once

#include "quant/core/ids.hpp"
#include "quant/core/result.hpp"
#include "quant/execution/execution_gateway.hpp"
#include "quant/execution/order_manager.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/risk/risk_manager.hpp"

namespace quant::execution {

class OrderRouter {
public:
  explicit OrderRouter(risk::RiskManager &risk_manager, OrderManager &order_manager);
  ~OrderRouter() = default;

  template <ExecutionGateway Gateway>
  [[nodiscard]] core::OrderId route_order(Gateway &gateway,
                                          const OrderRequest &request,
                                          core::Quantity current_position) {
    const auto risk_result = risk_manager_.check_order(request, current_position);
    if (!risk_result.is_approved()) {
      return core::OrderId{0}; // Rejected by risk check
    }

    const auto order_id = gateway.submit(request);
    if (order_id.value != 0) {
      order_manager_.track_order(request, order_id);
    }
    return order_id;
  }

  template <ExecutionGateway Gateway>
  void route_cancel(Gateway &gateway, core::OrderId order_id) {
    const auto risk_result = risk_manager_.check_cancellation(order_id);
    if (risk_result.is_approved()) {
      gateway.cancel(order_id);
    }
  }

private:
  risk::RiskManager &risk_manager_;
  OrderManager &order_manager_;
};

} // namespace quant::execution
