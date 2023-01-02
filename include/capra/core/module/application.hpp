#pragma once

#include "capra/core/module/module.hpp"
#include "capra/core/module/glcontext.hpp"
#include "capra/core/module/inputmgr.hpp"
#include "capra/core/module/window.hpp"
#include <memory>

namespace capra {

class Application : public Module {
public:
  std::shared_ptr<GLContext> ctx{nullptr};
  std::shared_ptr<InputMgr> input{nullptr};
  std::shared_ptr<Window> window{nullptr};

  Application();

  virtual void initialize();
  virtual void deinitialize();
  virtual void update(double dt);
  virtual void draw();

private:
  void received_msg_(const Msg &msg) override;
  void initialize_dependencies_(std::shared_ptr<Engine> engine) override;
};

template<> struct ModuleInfo<ModuleTag::Application> {
  const static std::vector<ModuleTag> dependencies;
};

} // namespace capra
