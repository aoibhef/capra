#include "capra/capra.hpp"
#include "fmt/format.h"

int main(int, char *[]) {
  fmt::print("4 + 5 = {}\n", add(4, 5));
}
