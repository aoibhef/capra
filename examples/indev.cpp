#define CAPRA_PRINT_HELPERS
#include "capra/capra.hpp"

class Indev : public capra::Application {
public:
  void initialize() override {
    window->open({
        .title = "Indev",
        .size = {800, 600},
        .flags = capra::WindowFlags::centered
    });
  }

  void deinitialize() override {}

  void update(double dt) override {
    if (input->down("a", 0.5)) {
      capra::fprintln("You pressed A!");
    }
  }

  void draw() override {}
};

CAPRA_RUN(Indev)
