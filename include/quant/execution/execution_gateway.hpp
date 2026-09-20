#pragma once

#include "quant/core/ids.hpp"
#include "quant/execution/order_request.hpp"

namespace quant::execution {

template <typename T>
concept ExecutionGateway =
    requires(T &gateway, const OrderRequest &request, core::OrderId order_id) {
      { gateway.submit(request) } -> std::same_as<core::OrderId>;
      { gateway.cancel(order_id) } -> std::same_as<void>;
    };

} // namespace quant::execution