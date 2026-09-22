#pragma once

#include <concepts>

#include "quant/market/market_tick.hpp"
#include "quant/market/order_book.hpp"
#include "quant/market/trade.hpp"

namespace quant::market {

template <typename T>
concept MarketDataListener = requires(T &listener, const MarketTick &tick,
                                      const Trade &trade,
                                      const Level2Book &book) {
  { listener.on_market_tick(tick) } noexcept -> std::same_as<void>;
  { listener.on_trade(trade) } noexcept -> std::same_as<void>;
  { listener.on_order_book(book) } noexcept -> std::same_as<void>;
};

} // namespace quant::market
