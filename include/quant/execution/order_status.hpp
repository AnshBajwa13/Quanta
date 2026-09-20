#include <cstdint>

namespace quant::execution {

enum class OrderStatus : std::uint8_t {
  New,
  Accepted,
  PartiallyFilled,
  Filled,
  CancelPending,
  Cancelled,
  Rejected,
  Expired
};

constexpr bool is_terminal(OrderStatus status) noexcept {
  return status == OrderStatus::Filled || status == OrderStatus::Cancelled ||
         status == OrderStatus::Rejected || status == OrderStatus::Expired;
}

} // namespace quant::execution