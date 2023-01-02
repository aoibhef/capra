#pragma once

#include "capra/core/msg_bus.hpp"
#include "fmt/format.h"
#include <string>

namespace capra {

class Engine;

enum class ModuleTag {
  Application,
  InputMgr,
  Window
};

class Module {
  friend class Engine;

public:
  ModuleTag tag;
  std::shared_ptr<Engine> engine{nullptr};

  explicit Module(ModuleTag tag);

  void poll_msgs();

protected:
  std::string msg_endpoint_id_;

  virtual void received_msg_(const Msg &msg) = 0;
  virtual void initialize_dependencies_(std::shared_ptr<Engine> engine);
};

template<ModuleTag>
struct ModuleInfo {
  const static std::vector<ModuleTag> dependencies;
};

} // namespace capra

template<> struct fmt::formatter<capra::ModuleTag>: formatter<string_view> {
  template <typename FormatContext>
  auto format(capra::ModuleTag t, FormatContext& ctx) const {
#define STRINGIFY(e) case (e): name = #e; break;
    string_view name = "capra::ModuleTag::?";
    switch (t) {
      using enum capra::ModuleTag;
      STRINGIFY(Application)
      STRINGIFY(InputMgr)
      STRINGIFY(Window)
    }
    return formatter<string_view>::format(name, ctx);
  }
};
