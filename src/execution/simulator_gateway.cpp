#include "quant/execution/simulator_gateway.hpp"

namespace quant::execution {

core::OrderId SimulatorGateway::submit(const OrderRequest &request) noexcept {
  if (simulator_ != nullptr) {
    return simulator_->submit(request);
  }
  return core::OrderId{next_order_id_++};
}

void SimulatorGateway::cancel(core::OrderId order_id) noexcept {
  if (simulator_ != nullptr) {
    simulator_->cancel(order_id);
  }
}

} // namespace quant::execution