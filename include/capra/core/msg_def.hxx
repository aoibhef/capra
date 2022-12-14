// AUTOGENERATED FILE, DO NOT EDIT
// See: tools/gen_msg_def.py
//      include/capra/core/msg_data.hpp

#pragma once

#include "capra/core/msg_data.hpp"
#include "fmt/format.h"
#include <variant>

namespace capra {

enum class MsgType {
  Initialize,
  Update,
  StartFrame,
  Draw,
  EndFrame,
  Shutdown,
  GlfwWindowClose,
  GlfwWindowSize,
  GlfwFramebufferSize,
  GlfwWindowContentScale,
  GlfwWindowPos,
  GlfwWindowIconify,
  GlfwWindowMaximize,
  GlfwWindowFocus,
  GlfwWindowRefresh,
  GlfwMonitor,
  GlfwKey,
  GlfwCharacter,
  GlfwCursorPos,
  GlfwCursorEnter,
  GlfwMouseButton,
  GlfwScroll,
  GlfwJoystick,
  GlfwDrop
};

template<MsgType>
struct Map;

template<> struct Map<MsgType::Initialize> {
  using type = Initialize;
};

template<> struct Map<MsgType::Update> {
  using type = Update;
};

template<> struct Map<MsgType::StartFrame> {
  using type = StartFrame;
};

template<> struct Map<MsgType::Draw> {
  using type = Draw;
};

template<> struct Map<MsgType::EndFrame> {
  using type = EndFrame;
};

template<> struct Map<MsgType::Shutdown> {
  using type = Shutdown;
};

template<> struct Map<MsgType::GlfwWindowClose> {
  using type = GlfwWindowClose;
};

template<> struct Map<MsgType::GlfwWindowSize> {
  using type = GlfwWindowSize;
};

template<> struct Map<MsgType::GlfwFramebufferSize> {
  using type = GlfwFramebufferSize;
};

template<> struct Map<MsgType::GlfwWindowContentScale> {
  using type = GlfwWindowContentScale;
};

template<> struct Map<MsgType::GlfwWindowPos> {
  using type = GlfwWindowPos;
};

template<> struct Map<MsgType::GlfwWindowIconify> {
  using type = GlfwWindowIconify;
};

template<> struct Map<MsgType::GlfwWindowMaximize> {
  using type = GlfwWindowMaximize;
};

template<> struct Map<MsgType::GlfwWindowFocus> {
  using type = GlfwWindowFocus;
};

template<> struct Map<MsgType::GlfwWindowRefresh> {
  using type = GlfwWindowRefresh;
};

template<> struct Map<MsgType::GlfwMonitor> {
  using type = GlfwMonitor;
};

template<> struct Map<MsgType::GlfwKey> {
  using type = GlfwKey;
};

template<> struct Map<MsgType::GlfwCharacter> {
  using type = GlfwCharacter;
};

template<> struct Map<MsgType::GlfwCursorPos> {
  using type = GlfwCursorPos;
};

template<> struct Map<MsgType::GlfwCursorEnter> {
  using type = GlfwCursorEnter;
};

template<> struct Map<MsgType::GlfwMouseButton> {
  using type = GlfwMouseButton;
};

template<> struct Map<MsgType::GlfwScroll> {
  using type = GlfwScroll;
};

template<> struct Map<MsgType::GlfwJoystick> {
  using type = GlfwJoystick;
};

template<> struct Map<MsgType::GlfwDrop> {
  using type = GlfwDrop;
};

struct Msg {
  MsgType type;
  std::variant<
    Initialize,
    Update,
    StartFrame,
    Draw,
    EndFrame,
    Shutdown,
    GlfwWindowClose,
    GlfwWindowSize,
    GlfwFramebufferSize,
    GlfwWindowContentScale,
    GlfwWindowPos,
    GlfwWindowIconify,
    GlfwWindowMaximize,
    GlfwWindowFocus,
    GlfwWindowRefresh,
    GlfwMonitor,
    GlfwKey,
    GlfwCharacter,
    GlfwCursorPos,
    GlfwCursorEnter,
    GlfwMouseButton,
    GlfwScroll,
    GlfwJoystick,
    GlfwDrop
  > data;
};

} // namespace capra

template<> struct fmt::formatter<capra::MsgType>: formatter<string_view> {
  template <typename FormatContext>
  auto format(capra::MsgType t, FormatContext& ctx) const {
#define STRINGIFY(e) case (e): name = #e; break;
    string_view name = "capra::MsgType::?";
    switch (t) {
      using enum capra::MsgType;
      STRINGIFY(Initialize)
      STRINGIFY(Update)
      STRINGIFY(StartFrame)
      STRINGIFY(Draw)
      STRINGIFY(EndFrame)
      STRINGIFY(Shutdown)
      STRINGIFY(GlfwWindowClose)
      STRINGIFY(GlfwWindowSize)
      STRINGIFY(GlfwFramebufferSize)
      STRINGIFY(GlfwWindowContentScale)
      STRINGIFY(GlfwWindowPos)
      STRINGIFY(GlfwWindowIconify)
      STRINGIFY(GlfwWindowMaximize)
      STRINGIFY(GlfwWindowFocus)
      STRINGIFY(GlfwWindowRefresh)
      STRINGIFY(GlfwMonitor)
      STRINGIFY(GlfwKey)
      STRINGIFY(GlfwCharacter)
      STRINGIFY(GlfwCursorPos)
      STRINGIFY(GlfwCursorEnter)
      STRINGIFY(GlfwMouseButton)
      STRINGIFY(GlfwScroll)
      STRINGIFY(GlfwJoystick)
      STRINGIFY(GlfwDrop)
    }
    return formatter<string_view>::format(name, ctx);
  }
#undef STRINGIFY
};
