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

namespace capra {

#ifdef CAPRA_PRINT_HELPERS
template<typename ...T>
void print(fmt::format_string<T...> fmt, T &&...args) {
  fmt::print(fmt, std::forward<T>(args)...);
}

template<typename ...T>
void println(fmt::format_string<T...> fmt, T &&...args) {
  fmt::print(fmt, std::forward<T>(args)...);
  fmt::print("\n");
}

template<typename ...T>
void fprint(fmt::format_string<T...> fmt, T &&...args) {
  print(fmt, std::forward<T>(args)...);
  std::fflush(stdout);
}

template<typename ...T>
void fprintln(fmt::format_string<T...> fmt, T &&...args) {
  println(fmt, std::forward<T>(args)...);
  std::fflush(stdout);
}
#endif //CAPRA_PRINT_HELPERS

} // namespace capra

#ifndef CAPRA_NO_RUN_MACRO
#include <memory>

#define CAPRA_RUN(application)                  \
  int main(int, char *[]) {                     \
    auto e = std::make_shared<capra::Engine>(); \
    e->run<application>();                      \
  }
#endif //CAPRA_NO_RUN_MACRO