// Concord
//
// Copyright (c) 2021 VMware, Inc. All Rights Reserved.
//
// This product is licensed to you under the Apache 2.0 license (the "License").
// You may not use this product except in compliance with the Apache 2.0 License.
//
// This product may include a number of subcomponents with separate copyright
// notices and license terms. Your use of these subcomponents is subject to
// the terms and conditions of the subcomponent's license, as noted in the
// LICENSE file.

#pragma once

#include "Logger.hpp"
#include "assertUtils.hpp"

namespace concord::util {

class ProgressLogger {
  logging::Logger& logger_;
  std::string msg_;
  uint32_t log_step_;
  uint64_t final_val_;

 public:
  ProgressLogger(logging::Logger& logger,
                 std::string_view msg,
                 const uint64_t initial_val,
                 const uint64_t final_val,
                 const uint8_t step_percentage)
      : logger_{logger}, msg_{msg}, final_val_{final_val} {
    ConcordAssertGT(step_percentage, 0);
    ConcordAssertLE(step_percentage, 100);

    uint64_t range = final_val > initial_val ? (final_val - initial_val) : (initial_val - final_val); // take absolute value
    log_step_ = range / step_percentage / 100;
    if (log_step_ == 0) log_step_ = 1;
  }

  ProgressLogger(logging::Logger& logger, std::string_view msg, const uint64_t initial_val, const uint64_t final_val)
      : ProgressLogger(logger, msg, initial_val, final_val, 10) {}

  void logProgress(uint64_t current_val) {
    if (log_step_ == 0) {
      // the value range is too small to log
      return;
    }

    if (current_val % log_step_ == 0) {
      LOG_INFO(logger_, msg_ << current_val << " -> " << final_val_);
    }
  }
};
}  // namespace concord::util