#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <new>
#include <optional>
#include <utility>

namespace quant::core {

// Standard cache line size across modern x86_64 / ARM64 architectures
constexpr std::size_t CachelineSize = 64;

/// Lock-free Single-Producer Single-Consumer (SPSC) ring buffer.
/// Pre-allocated, zero heap allocations on hot path, cacheline-aligned to prevent false sharing.
template <typename T, std::size_t Capacity>
class SpscQueue {
  static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be a power of 2");

public:
  SpscQueue() = default;
  ~SpscQueue() = default;

  SpscQueue(const SpscQueue &) = delete;
  SpscQueue &operator=(const SpscQueue &) = delete;
  SpscQueue(SpscQueue &&) = delete;
  SpscQueue &operator=(SpscQueue &&) = delete;

  [[nodiscard]] bool try_push(const T &item) noexcept {
    const auto current_tail = tail_.load(std::memory_order_relaxed);
    if ((current_tail - cached_head_) >= Capacity) {
      cached_head_ = head_.load(std::memory_order_acquire);
      if ((current_tail - cached_head_) >= Capacity) {
        return false;
      }
    }

    buffer_[current_tail & Mask] = item;
    tail_.store(current_tail + 1, std::memory_order_release);
    return true;
  }

  [[nodiscard]] bool try_push(T &&item) noexcept {
    const auto current_tail = tail_.load(std::memory_order_relaxed);
    if ((current_tail - cached_head_) >= Capacity) {
      cached_head_ = head_.load(std::memory_order_acquire);
      if ((current_tail - cached_head_) >= Capacity) {
        return false;
      }
    }

    buffer_[current_tail & Mask] = std::move(item);
    tail_.store(current_tail + 1, std::memory_order_release);
    return true;
  }

  template <typename... Args>
  [[nodiscard]] bool emplace(Args &&...args) noexcept {
    const auto current_tail = tail_.load(std::memory_order_relaxed);
    if ((current_tail - cached_head_) >= Capacity) {
      cached_head_ = head_.load(std::memory_order_acquire);
      if ((current_tail - cached_head_) >= Capacity) {
        return false;
      }
    }

    buffer_[current_tail & Mask] = T(std::forward<Args>(args)...);
    tail_.store(current_tail + 1, std::memory_order_release);
    return true;
  }

  [[nodiscard]] bool try_pop(T &item) noexcept {
    const auto current_head = head_.load(std::memory_order_relaxed);
    if (current_head == cached_tail_) {
      cached_tail_ = tail_.load(std::memory_order_acquire);
      if (current_head == cached_tail_) {
        return false;
      }
    }

    item = std::move(buffer_[current_head & Mask]);
    head_.store(current_head + 1, std::memory_order_release);
    return true;
  }

  [[nodiscard]] std::optional<T> try_pop() noexcept {
    const auto current_head = head_.load(std::memory_order_relaxed);
    if (current_head == cached_tail_) {
      cached_tail_ = tail_.load(std::memory_order_acquire);
      if (current_head == cached_tail_) {
        return std::nullopt;
      }
    }

    T item = std::move(buffer_[current_head & Mask]);
    head_.store(current_head + 1, std::memory_order_release);
    return item;
  }

  [[nodiscard]] bool empty() const noexcept {
    return head_.load(std::memory_order_relaxed) == tail_.load(std::memory_order_relaxed);
  }

  [[nodiscard]] std::size_t size() const noexcept {
    const auto h = head_.load(std::memory_order_relaxed);
    const auto t = tail_.load(std::memory_order_relaxed);
    return (t >= h) ? (t - h) : (Capacity - (h - t));
  }

  [[nodiscard]] static constexpr std::size_t capacity() noexcept {
    return Capacity;
  }

private:
  static constexpr std::size_t Mask = Capacity - 1;

  alignas(CachelineSize) std::atomic<std::size_t> tail_{0};
  std::size_t cached_head_{0};

  alignas(CachelineSize) std::atomic<std::size_t> head_{0};
  std::size_t cached_tail_{0};

  alignas(CachelineSize) std::array<T, Capacity> buffer_{};
};

} // namespace quant::core
