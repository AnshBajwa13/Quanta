#include "quant/adapters/upstox/upstox_execution_gateway.hpp"

namespace quant::adapters::upstox {

core::OrderId UpstoxExecutionGateway::submit(const execution::OrderRequest & /*request*/) noexcept {
  // SKELETON: Upstox Order Placement API call to be implemented
  return core::OrderId{next_order_id_++};
}

void UpstoxExecutionGateway::cancel(core::OrderId /*order_id*/) noexcept {
  // SKELETON: Upstox Order Cancellation API call to be implemented
}

void UpstoxExecutionGateway::set_listener(execution::IExecutionListener *listener) noexcept {
  listener_ = listener;
}

} // namespace quant::adapters::upstox
