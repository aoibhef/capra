#pragma once

#include "capra/core/module/module.hpp"
#include "capra/core/msg_bus.hpp"
#include <functional>
#include <unordered_map>

namespace capra {

class InputMgr : public Module {
public:
  struct {
    double x{0.0};
    double y{0.0};
    double px{0.0};
    double py{0.0};
    double dx{0.0};
    double dy{0.0};

    double sx{0.0};
    double sy{0.0};

    bool moved{false};
    bool entered{false};

    bool got_first_event{false};
  } mouse{};

  InputMgr();

  void update(double dt);

  void bind(const std::string &action, const std::string &binding);
  void unbind(const std::string &binding);

  void bind_func(const std::string &action, const std::function<void(void)> &f);

  bool pressed(const std::string &binding);
  bool released(const std::string &binding);
  bool down(const std::string &binding, double interval = 0.0, double delay = 0.0);

private:
  void received_msg_(const Msg &msg) override;

  struct Repeat {
    double time;
    double interval;
    double delay{0.0};
    bool delay_stage{false};
    bool pressed{false};
  };

  std::unordered_map<std::string, std::string> bindings_{};
  std::unordered_map<std::string, std::function<void(void)>> func_bindings_{};
  std::unordered_map<std::string, bool> state_{};
  std::unordered_map<std::string, bool> prev_state_{};
  std::unordered_map<std::string, Repeat> repeat_state_{};

  std::string glfw_key_to_str_(int key);
  std::string glfw_button_to_str_(int button);
  const std::unordered_set<std::string> &all_actions_();
};

template<> struct ModuleInfo<ModuleTag::InputMgr> {
  const static std::vector<ModuleTag> dependencies;
};

} // namespace capra
