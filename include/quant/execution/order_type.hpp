#pragma once

#include <cstdint>

namespace quant::execution {

enum class OrderType : std::uint8_t { Market, Limit, Stop, StopLimit };

} // namespace quant::execution