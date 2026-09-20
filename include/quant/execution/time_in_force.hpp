#pragma once

#include <cstdint>

namespace quant::execution {

enum class TimeInForce : std::uint8_t { Day, IOC, FOK };

} // namespace quant::execution