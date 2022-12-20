#pragma once

#include "capra/core/module/application.hpp"
#include "capra/core/module/inputmgr.hpp"
#include "capra/core/module/module.hpp"
#include "capra/core/module/window.hpp"
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace capra {

class Engine {
public:
  Engine();

  template<class T, typename... Args> requires std::derived_from<T, Module>
  void add_module(ModuleTag tag, const Args &&...args);

  template<class T> requires std::derived_from<T, Module>
  std::shared_ptr<T> get_module(ModuleTag tag) const;

  template<class T> requires std::derived_from<T, Application>
  void run();

private:
  std::string msg_endpoint_id_;
  std::unordered_map<ModuleTag, std::shared_ptr<Module>> modules_{};
  std::vector<ModuleTag> module_poll_prio_{};

  bool received_shutdown_{false};

  void received_msg_(const Msg &msg);

  static std::once_flag initialized_glfw_;
};

template<class T, typename... Args> requires std::derived_from<T, Module>
void Engine::add_module(ModuleTag tag, const Args &&...args) {
  modules_[tag] = std::shared_ptr<Module>(new T(*this, args...));
  module_poll_prio_.emplace_back(tag);
}

template<class T> requires std::derived_from<T, Module>
std::shared_ptr<T> Engine::get_module(ModuleTag tag) const {
  return std::dynamic_pointer_cast<T>(modules_.at(tag));
}

template<class T> requires std::derived_from<T, Application>
void Engine::run() {
  add_module<Window>(ModuleTag::Window);
  add_module<InputMgr>(ModuleTag::InputMgr);
  add_module<T>(ModuleTag::Application);

  MsgBus::send_nowait<MsgType::Initialize>();

  do {
    MsgBus::send_nowait<MsgType::Update>(0.0);

    MsgBus::send_nowait<MsgType::StartFrame>();
    MsgBus::send_nowait<MsgType::Draw>();
    MsgBus::send_nowait<MsgType::EndFrame>();

    for (const auto tag : module_poll_prio_)
      modules_[tag]->poll_msgs();
    MsgBus::poll(msg_endpoint_id_);

    glfwPollEvents();
  } while (!received_shutdown_);
}

} // namespace capra
