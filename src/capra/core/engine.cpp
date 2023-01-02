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

void Engine::resolve_module_dependencies_() {
  std::unordered_set<ModuleTag> modules_to_initialize{};
  for (const auto &m : module_dependencies_)
    modules_to_initialize.insert(m.first);

  std::vector<ModuleTag> resolution_order{};

  while (!modules_to_initialize.empty()) {
    for (auto it = modules_to_initialize.begin(); it != modules_to_initialize.end(); ) {
      const auto &dependencies = module_dependencies_[*it];

      bool all_resolved = true;
      for (const auto &dep : dependencies)
        if (std::find(resolution_order.begin(), resolution_order.end(), dep) == resolution_order.end()) {
          all_resolved = false;
          break;
        }

      if (all_resolved) {
        resolution_order.emplace_back(*it);
        it = modules_to_initialize.erase(it);
      } else
        it++;
    }
  }

  CAPRA_LOG_DEBUG("Module dependency resolution order: {}", resolution_order);

  for (const auto &tag : resolution_order)
    modules_[tag]->initialize_dependencies_(shared_from_this());
}

} // namespace capra
