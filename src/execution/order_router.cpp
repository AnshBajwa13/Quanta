#include "quant/execution/order_router.hpp"

namespace quant::execution {

OrderRouter::OrderRouter(risk::RiskManager &risk_manager, OrderManager &order_manager)
    : risk_manager_(risk_manager), order_manager_(order_manager) {}

} // namespace quant::execution
