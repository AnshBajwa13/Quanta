#pragma once

#include <cstdint>

#include "quant/core/ids.hpp"
#include "quant/execution/execution_gateway.hpp"
#include "quant/execution/execution_listener.hpp"
#include "quant/execution/order_request.hpp"

namespace quant::adapters::upstox {

class UpstoxExecutionGateway {
public:
  UpstoxExecutionGateway() = default;
  ~UpstoxExecutionGateway() = default;

  core::OrderId submit(const execution::OrderRequest &request) noexcept;
  void cancel(core::OrderId order_id) noexcept;

  void set_listener(execution::IExecutionListener *listener) noexcept;

private:
  execution::IExecutionListener *listener_{nullptr};
  std::uint64_t next_order_id_{1};
};

static_assert(execution::ExecutionGateway<UpstoxExecutionGateway>);

} // namespace quant::adapters::upstox
