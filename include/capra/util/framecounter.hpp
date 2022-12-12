#pragma once

#include "capra/util/averagers.hpp"
#include "capra/util/ticker.hpp"
#include "capra/util/time.hpp"
#include <cstdint>
#include <deque>
#include <vector>

namespace capra {

class FrameCounter {
public:
  FrameCounter();

  void update();

  double fps() const;
  double dt() const;

  std::vector<double> dts_vec() const;

private:
  std::uint64_t start_time_{};
  std::deque<std::uint64_t> timestamps_{};
  std::deque<double> dts_{};

  Ticker ticker_{0.5};
  EMA averager_{1.0};
};

} // namespace capra
