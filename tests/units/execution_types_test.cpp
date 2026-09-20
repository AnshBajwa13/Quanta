#include <cassert>
#include <cstdint>
#include <type_traits>

#include "quant/execution/order_status.hpp"
#include "quant/execution/order_type.hpp"
#include "quant/execution/side.hpp"
#include "quant/execution/time_in_force.hpp"

int main() {
  using namespace quant::execution;

  Side side = Side::Buy;
  OrderType type = OrderType::Limit;
  TimeInForce tif = TimeInForce::Day;
  OrderStatus status = OrderStatus::New;

  assert(side == Side::Buy);
  assert(type == OrderType::Limit);
  assert(tif == TimeInForce::Day);
  assert(status == OrderStatus::New);

  static_assert(std::is_same_v<std::underlying_type_t<Side>, std::uint8_t>);

  static_assert(
      std::is_same_v<std::underlying_type_t<OrderType>, std::uint8_t>);

  static_assert(
      std::is_same_v<std::underlying_type_t<TimeInForce>, std::uint8_t>);

  static_assert(
      std::is_same_v<std::underlying_type_t<OrderStatus>, std::uint8_t>);

  return 0;
}