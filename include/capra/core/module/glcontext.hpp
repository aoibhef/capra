#pragma once

#include "capra/core/module/module.hpp"
#include "capra/core/module/window.hpp"
#include "capra/gfx/color.hpp"
#include "capra/util/enum_bitops.hpp"
#include "glad/gl.h"

namespace capra {

enum class ClearBit {
  stencil = GL_STENCIL_BUFFER_BIT,
  color = GL_COLOR_BUFFER_BIT,
  depth = GL_DEPTH_BUFFER_BIT
};

} // namespace capra

ENABLE_BITOPS(capra::ClearBit);

namespace capra {

class GLContext : public Module {
public:
  GLContext();

  void initialize_from_window(const Window &window);

  void clear(const RGB &color, const ClearBit &bit = ClearBit::color | ClearBit::depth);

private:
  void received_msg_(const Msg &msg) override;

  static void GLAPIENTRY gl_message_callback_(
      GLenum source,
      GLenum type,
      GLuint id,
      GLenum severity,
      GLsizei length,
      const GLchar *message,
      const void *userParam
  );
};

template<> struct ModuleInfo<ModuleTag::GLContext> {
  const static std::vector<ModuleTag> dependencies;
};

} // namespace capra
