#include "capra/util/ticker.hpp"

#include "capra/util/time.hpp"

namespace capra {

Ticker::Ticker(double interval) : interval_(static_cast<std::uint64_t>(interval * 1e9)) {
  start_ = time_nsec();
  last_ = start_;
}

void Ticker::reset() {
  start_ = time_nsec();
  last_ = start_;
  dt_ = 0;
  acc_ = 0;
}

std::uint64_t Ticker::tick() {
  auto now = time_nsec();
  dt_ = now - last_;
  last_ = now;

  std::uint64_t tick_count = 0;

  if (interval_ > 0) {
    acc_ += dt_;
    while (acc_ >= interval_) {
      acc_ -= interval_;
      tick_count++;
    }
  }

  return tick_count;
}

double Ticker::dt_nsec() {
  return dt_;
}

double Ticker::dt_usec() {
  return dt_ / 1e3;
}

double Ticker::dt_msec() {
  return dt_ / 1e6;
}

double Ticker::dt_sec() {
  return dt_ / 1e9;
}

} // namespace capra
