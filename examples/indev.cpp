#include "capra/capra.hpp"
#include "capra/util/rnd.hpp"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

int main(int, char *[]) {
  fmt::print("Random int: {}", rnd::get<int>(10));
}