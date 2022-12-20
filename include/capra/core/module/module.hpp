#pragma once

#include "capra/core/msg_bus.hpp"
#include <string>

namespace capra {

class Engine;

enum class ModuleTag {
  Application,
  InputMgr,
  Window
};

class Module {
public:
  ModuleTag tag;
  const Engine &engine;

  explicit Module(const Engine &engine, ModuleTag tag);

  void poll_msgs();

protected:
  std::string msg_endpoint_id_;

  virtual void received_msg_(const Msg &msg) = 0;
};

} // namespace capra
