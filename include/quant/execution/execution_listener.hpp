#pragma once

#include <concepts>

#include "quant/execution/execution_report.hpp"
#include "quant/execution/fill.hpp"

namespace quant::execution {

template <typename T>
concept ExecutionListener = requires(T &listener, const ExecutionReport &report,
                                     const Fill &fill) {
  { listener.on_execution_report(report) } noexcept -> std::same_as<void>;
  { listener.on_fill(fill) } noexcept -> std::same_as<void>;
};

class IExecutionListener {
public:
  virtual ~IExecutionListener() = default;

  virtual void on_execution_report(const ExecutionReport &report) noexcept = 0;
  virtual void on_fill(const Fill &fill) noexcept = 0;
};

} // namespace quant::execution
