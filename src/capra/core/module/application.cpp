#include "capra/core/module/application.hpp"

#include "capra/core/engine.hpp"
#include "capra/util/log.hpp"
#include "capra/util/overloaded.hpp"

namespace capra {

const std::vector<ModuleTag> ModuleInfo<ModuleTag::Application>::dependencies = {ModuleTag::InputMgr, ModuleTag::Window};

Application::Application() : Module(ModuleTag::Application) {
  MsgBus::subscribe(msg_endpoint_id_, MsgType::Initialize);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::Update);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::StartFrame);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::EndFrame);
}

void Application::initialize() {}
void Application::deinitialize() {}
void Application::update(double dt) {}
void Application::draw() {}

void Application::initialize_dependencies_(std::shared_ptr<Engine> engine) {
  Module::initialize_dependencies_(engine);

  input = engine->get_module<InputMgr>(ModuleTag::InputMgr);
  window = engine->get_module<Window>(ModuleTag::Window);
}

void Application::received_msg_(const Msg &msg) {
  std::visit(overloaded {
      [&](const Initialize &) { initialize(); },
      [&](const Update &e) {
        update(e.dt);
        input->update(e.dt);
      },
      [&](const StartFrame &) { },
      [&](const EndFrame &) { window->swap(); },
      [&](const auto &e) { CAPRA_LOG_WARN("Application received unhandled event: {}", msg.type); }
  }, msg.data);
}

} // namespace capra
