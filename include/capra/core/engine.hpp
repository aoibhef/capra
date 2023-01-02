#pragma once

#include "capra/core/module/application.hpp"
#include "capra/core/module/inputmgr.hpp"
#include "capra/core/module/module.hpp"
#include "capra/core/module/window.hpp"
#include "capra/util/framecounter.hpp"
#include "capra/util/log.hpp"
#include "capra/util/ticker.hpp"
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace capra {

class Engine : public std::enable_shared_from_this<Engine> {
public:
  Engine();

  template<class T, ModuleTag tag, typename... Args> requires std::derived_from<T, Module>
  void add_module(const Args &&...args);

  template<class T> requires std::derived_from<T, Module>
  std::shared_ptr<T> get_module(ModuleTag tag) const;

  template<class T> requires std::derived_from<T, Application>
  void run();

  void shutdown();

private:
  std::string msg_endpoint_id_;
  std::unordered_map<ModuleTag, std::shared_ptr<Module>> modules_{};
  std::unordered_map<ModuleTag, std::vector<ModuleTag>> module_dependencies_{};
  std::vector<ModuleTag> module_poll_prio_{};

  bool received_shutdown_{false};

  void received_msg_(const Msg &msg);
  void resolve_module_dependencies_();

  static std::once_flag initialized_glfw_;
};

template<class T, ModuleTag tag, typename... Args> requires std::derived_from<T, Module>
void Engine::add_module(const Args &&...args) {
  modules_[tag] = std::shared_ptr<Module>(new T(args...));
  module_dependencies_[tag] = ModuleInfo<tag>::dependencies;
  module_poll_prio_.emplace_back(tag);
}

template<class T> requires std::derived_from<T, Module>
std::shared_ptr<T> Engine::get_module(ModuleTag tag) const {
  return std::dynamic_pointer_cast<T>(modules_.at(tag));
}

template<class T> requires std::derived_from<T, Application>
void Engine::run() {
  add_module<T, ModuleTag::Application>();

  add_module<GLContext, ModuleTag::GLContext>();
  add_module<InputMgr, ModuleTag::InputMgr>();
  add_module<Window, ModuleTag::Window>();

  // Modules are not fully initialized,
  // dependencies must be resolved first
  resolve_module_dependencies_();

  MsgBus::send_nowait<MsgType::Initialize>();

  FrameCounter framecounter{1.0};
  do {
    MsgBus::send_nowait<MsgType::Update>(framecounter.dt());

    MsgBus::send_nowait<MsgType::StartFrame>();
    MsgBus::send_nowait<MsgType::Draw>();
    MsgBus::send_nowait<MsgType::EndFrame>();

    for (const auto tag : module_poll_prio_)
      modules_[tag]->poll_msgs();
    MsgBus::poll(msg_endpoint_id_);

    glfwPollEvents();

    if (framecounter.update() > 0)
      CAPRA_LOG_DEBUG("FPS: {:.2f}", framecounter.fps());
  } while (!received_shutdown_);
}

} // namespace capra
