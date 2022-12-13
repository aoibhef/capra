#include "capra/core/engine.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "capra/util/log.hpp"
#include "capra/util/overloaded.hpp"

namespace capra {

std::once_flag Engine::initialized_glfw_{};

Engine::Engine() {
  std::call_once(initialized_glfw_, [&]{
    if (glfwInit() == GLFW_FALSE)
      std::exit(EXIT_FAILURE);

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    CAPRA_LOG_DEBUG("Initialized GLFW v{}.{}.{}", major, minor, revision);
  });

  msg_endpoint_id_ = MsgBus::register_endpoint([&](const Msg &msg) { received_msg_(msg); });
  MsgBus::subscribe(msg_endpoint_id_, MsgType::Shutdown);
}

void Engine::received_msg_(const Msg &msg) {
  std::visit(overloaded {
    [&](const Shutdown &e) { received_shutdown_ = true; },
    [&](const auto &e) { CAPRA_LOG_WARN("Engine received unhandled event: {}", msg.type); }
  }, msg.data);
}

} // namespace capra
