#include "capra/capra.hpp"

class Indev : public capra::Application {
public:
  explicit Indev(capra::Engine &engine) : Application(engine) {}

  void initialize() override {
    window->open({
        .title = "Indev",
        .size = {800, 600},
        .flags = capra::WindowFlags::centered
    });
  }

  void deinitialize() override {}

  void update(double dt) override {
    if (input->pressed("a")) {
      fmt::print("You pressed A!\n");
      std::fflush(stdout);
    }
  }

  void draw() override {}
};

CAPRA_RUN(Indev)
