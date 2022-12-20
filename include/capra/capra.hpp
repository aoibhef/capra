#pragma once

#include "capra/core/engine.hpp"

#include "capra/util/noise/perlin.hpp"
#include "capra/util/averagers.hpp"
#include "capra/util/log.hpp"
#include "capra/util/platform.hpp"
#include "capra/util/rnd.hpp"
#include "capra/util/ticker.hpp"
#include "capra/util/time.hpp"

#include "fmt/format.h"
#include "fmt/ranges.h"

#define CAPRA_RUN(application)          \
  int main(int, char *[]) {             \
    capra::Engine().run<application>(); \
  }
