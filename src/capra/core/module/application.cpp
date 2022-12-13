#include "capra/core/module/application.hpp"

#include "capra/core/engine.hpp"
#include "capra/util/log.hpp"
#include "capra/util/overloaded.hpp"

namespace capra {

Application::Application(const Engine &engine) : Module(engine, ModuleTag::Application) {
  window = engine.get_module<Window>(ModuleTag::Window);

  MsgBus::subscribe(msg_endpoint_id_, MsgType::Initialize);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::StartFrame);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::EndFrame);
}

void Application::initialize() {}
void Application::deinitialize() {}
void Application::update(double dt) {}
void Application::draw() {}

void Application::received_msg_(const Msg &msg) {
  std::visit(overloaded {
      [&](const Initialize &) { initialize(); },
      [&](const StartFrame &) { },
      [&](const EndFrame &) { window->swap(); },
      [&](const auto &e) { CAPRA_LOG_WARN("Engine received unhandled event: {}", msg.type); }
  }, msg.data);
}

} // namespace capra
