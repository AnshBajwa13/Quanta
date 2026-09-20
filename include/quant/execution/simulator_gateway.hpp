#pragma once

#include <cstdint>

#include "quant/core/ids.hpp"
#include "quant/execution/execution_gateway.hpp"
#include "quant/execution/order_request.hpp"

namespace quant::execution {

class SimulatorGateway {
public:
  core::OrderId submit(const OrderRequest &request) noexcept;

  void cancel(core::OrderId order_id) noexcept;

private:
  std::uint64_t next_order_id_{1};
};

static_assert(ExecutionGateway<SimulatorGateway>);

} // namespace quant::execution