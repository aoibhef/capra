#include "capra/core/module/module.hpp"

#include "capra/core/engine.hpp"

namespace capra {

Module::Module(const Engine &engine, const ModuleTag tag) : tag(tag), engine(engine) {
  msg_endpoint_id_ = MsgBus::register_endpoint([&](const Msg &msg) { received_msg_(msg); });
}

void Module::poll_msgs() {
  MsgBus::poll(msg_endpoint_id_);
}

} // namespace capra
