#include "capra/core/module/module.hpp"

#include "capra/core/engine.hpp"

namespace capra {

Module::Module(const ModuleTag tag) : tag(tag) {
  msg_endpoint_id_ = MsgBus::register_endpoint([&](const Msg &msg) { received_msg_(msg); });
}

void Module::poll_msgs() {
  MsgBus::poll(msg_endpoint_id_);
}

void Module::initialize_dependencies_(std::shared_ptr<Engine> engine) {
  this->engine = engine;
}

} // namespace capra
