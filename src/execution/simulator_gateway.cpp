#include "quant/execution/simulator_gateway.hpp"

namespace quant::execution {

core::OrderId SimulatorGateway::submit(const OrderRequest & /*request*/
                                       ) noexcept {
  return core::OrderId{next_order_id_++};
}

void SimulatorGateway::cancel(core::OrderId /*order_id*/
                              ) noexcept {}

} // namespace quant::execution