/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 */

#include <chrono>
#include <cmath>
#include <ostream>

#pragma once

namespace quic {

using namespace std::chrono_literals;

struct Bandwidth {
  uint64_t units{0};
  std::chrono::microseconds interval{0us};

  explicit Bandwidth()
      : units(0),
        interval(std::chrono::microseconds::zero()),
        unitName_("bytes") {}

  explicit Bandwidth(
      uint64_t unitsDelievered,
      std::chrono::microseconds deliveryInterval)
      : units(unitsDelievered),
        interval(deliveryInterval),
        unitName_("bytes") {}

  explicit Bandwidth(
      uint64_t unitsDelievered,
      std::chrono::microseconds deliveryInterval,
      std::string unitName)
      : units(unitsDelievered),
        interval(deliveryInterval),
        unitName_(std::move(unitName)) {}

  explicit operator bool() const noexcept {
    return units != 0;
  }

  template <
      typename T,
      typename = std::enable_if_t<std::is_arithmetic<T>::value>>
  const Bandwidth operator*(T t) const noexcept {
    return Bandwidth(std::ceil(units * t), interval);
  }

  template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
  const Bandwidth operator/(T t) const noexcept {
    return Bandwidth(units / t, interval);
  }

  uint64_t operator*(std::chrono::microseconds delay) const noexcept {
    return interval == std::chrono::microseconds::zero()
        ? 0
        : units * delay / interval;
  }

  const std::string& unitName() const noexcept;

  std::string conciseDescribe() const noexcept;

 private:
  std::string unitName_;
};

bool operator<(const Bandwidth& lhs, const Bandwidth& rhs);
bool operator<=(const Bandwidth& lhs, const Bandwidth& rhs);
bool operator>(const Bandwidth& lhs, const Bandwidth& rhs);
bool operator>=(const Bandwidth& lhs, const Bandwidth& rhs);
bool operator==(const Bandwidth& lhs, const Bandwidth& rhs);

uint64_t operator*(std::chrono::microseconds delay, const Bandwidth& bandwidth);
std::ostream& operator<<(std::ostream& os, const Bandwidth& bandwidth);
} // namespace quic
