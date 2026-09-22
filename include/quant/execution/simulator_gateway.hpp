#pragma once

#include <cstdint>

#include "quant/core/ids.hpp"
#include "quant/execution/execution_gateway.hpp"
#include "quant/execution/order_request.hpp"
#include "quant/execution/simulator.hpp"

namespace quant::execution {

class SimulatorGateway {
public:
  explicit SimulatorGateway(Simulator *simulator = nullptr) noexcept
      : simulator_(simulator) {}

  void set_listener(IExecutionListener *listener) noexcept {
    if (simulator_ != nullptr) {
      simulator_->set_listener(listener);
    }
  }

  core::OrderId submit(const OrderRequest &request) noexcept;

  void cancel(core::OrderId order_id) noexcept;

private:
  Simulator *simulator_{nullptr};
  std::uint64_t next_order_id_{1};
};

static_assert(ExecutionGateway<SimulatorGateway>);

} // namespace quant::execution