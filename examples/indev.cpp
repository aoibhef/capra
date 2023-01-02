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
    ctx->initialize_from_window(*window);
  }

  void deinitialize() override {}

  void update(double dt) override {
    if (input->pressed("escape"))
      engine->shutdown();
  }

  void draw() override {
    ctx->clear(capra::rgb(255, 255, (input->mouse.x / (double)window->w) * 255));
  }
};

CAPRA_RUN(Indev)
