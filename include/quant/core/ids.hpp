#pragma once

#include <cstdint>

namespace quant::core {

struct OrderId {
  std::uint64_t value{};
};

struct InstrumentId {
  std::uint64_t value{};
};

struct ExecutionId {
  std::uint64_t value{};
};

struct StrategyId {
  std::uint64_t value{};
};

} // namespace quant::core